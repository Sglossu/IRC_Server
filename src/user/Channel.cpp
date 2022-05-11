#include "Channel.hpp"
#include "../handler/Handler.hpp"

Channel::Channel(std::string name, User &user, std::string pass, Handler *handler) {
	_name = name;
	_flags = 0;
	if (!pass.empty()) {
		_flags |= HAS_PASS;
		_pass = pass;
	}
	_handler = handler;
}

Channel::~Channel() {
	//todo: удаляем всех пользователей с группы через KICK?
}


void Channel::_delete_user(const std::string &userNick) {
	std::vector<std::string>::iterator it;

	it = std::find(begin(_users), end(_users), userNick);
	if (it != _users.end()) {
		// todo: send user left the channel
		_users.erase(it);
	}
	it = std::find(begin(_operators), end(_operators), userNick);
	if (it != _operators.end()) {
		// todo: send user left the channel
		_operators.erase(it);
	}
	if (!_operators.size() and _users.size())
		_operators.push_back(_users[0]);
}

void	Channel::_join_user(User &user, std::string pass, bool after_invite) {
	// check if invite-only
	if (!after_invite && (_flags & INVITE_ONLY)) {
		_handler->_error_msg(user, 473, _name);
		return ;
	}
	// check ban
	if (_flags & BAN_ALL)
		_handler->_error_msg(user, 474, _name);
	// check pass
	if (!after_invite && (_flags & HAS_PASS) && _pass.compare(pass))
		_handler->_error_msg(user, 475, _name);
	else if (user.getChanels().size() > 10) {
		_handler->_error_msg(user, 405, _name);
	}
	// если такой пользователь уже есть
	else if(_is_user_on_channel(user.getNick()))
		_handler->_error_msg(user, 443, _name);
	else {
		if (!_operators.size())
			_operators.push_back(user.getNick());
		user.join_channel(_name);
		_users.push_back(user.getNick());
		if (!after_invite) {
			std::string ms = "JOIN " + _name;
			_handler->_write_to_channel(_name, user, ms);
			_return_topic(user);
		}
//		else {
//			std::string ms = user.getNick() + "!" + user.getNick() + "@" + "127.0.0.1" + "JOIN" + _name + "\r\n";
//			_handler->_write_to_channel(_name, user, ms);
//		}
	}
}

void Channel::_return_topic(User &user) {
	// todo поменять 127,0,0,1 на вызов переменной

	if (_topic.empty())
		_handler->_cmd_responses(_name, user, 331);
	else
		_handler->_cmd_responses(_name + " :" + _topic, user, 332);

//	std::string names = user.getNick() + " = " + _name + " :";
//	for (size_t i = 0; i < _operators.size(); i++) {
//		if (i == 0)
//			names += "@" + _operators[i];
//		else
//			names += " @" + _operators[i];
//	}
//	for (size_t i = _operators.size(); i < _users.size(); i++) {
//		if (i == 0 && !_operators.size())
//			names += _users[i];
//		else
//			names += " " + _users[i];
//	}
	std::string names = _namreply(user);
	_handler->_cmd_responses(names, user, 353);
	_handler->_cmd_responses(_name, user, 366);
	return ;
}

std::string		Channel::_namreply(User &user) {
	std::string names = user.getNick() + " = " + _name + " :";
	for (size_t i = 0; i < _operators.size(); i++) {
		if (i == 0)
			names += "@" + _operators[i];
		else
			names += " @" + _operators[i];
	}
	for (size_t i = _operators.size(); i < _users.size(); i++) {
		if (i == 0 && !_operators.size())
			names += _users[i];
		else
			names += " " + _users[i];
	}
	return names;
}

bool	Channel::_is_user_on_channel(std::string nick) {
	if (std::find(_users.begin(), _users.end(), nick) != _users.end() ||
			std::find(_operators.begin(), _operators.end(), nick) != _operators.end())
		return true;
	return false;
}

bool	Channel::_is_user_operator(std::string nick) {
	if (std::find(_operators.begin(), _operators.end(), nick) != _operators.end())
		return true;
	return false;
}

bool	Channel::_is_user_in_banlist(std::string nick) {
	if (std::find(_ban_lists.begin(), _ban_lists.end(), nick) != _ban_lists.end())
		return true;
	return false;
}

const std::string &Channel::getName() const {
	return _name;
}

void Channel::setName(const std::string &name) {
	_name = name;
}

unsigned char Channel::getFlags() const {
	return _flags;
}

void Channel::setFlags(unsigned char flag) {
	_flags |= flag;
}

const std::vector<std::string> &Channel::getUsers() const {
	return _users;
}

const std::vector<std::string> &Channel::getOperators() const {
	return _operators;
}

void Channel::setOperators(std::string &nick_user) {
	_operators.push_back(nick_user);
}

void Channel::setInBanList(std::string &nick_user) {
	_ban_lists.insert(nick_user);
}

void Channel::delFromBanList(std::string &nick_user) {
	_ban_lists.erase(nick_user);
}

void Channel::delOperators(std::string &nick_user) {
	for (std::vector<std::string>::iterator it = _operators.begin(); it != _operators.end(); it++) {
		if (*it == nick_user) {
			_operators.erase(it);
			return ;
		}
	}
}

void	Channel::delFlag(unsigned char flag) {
	_flags &= ~flag;
}

const std::string &Channel::getPass() const {
	return _pass;
}

void Channel::setPass(const std::string &pass) {
	_pass = pass;
}

const std::string &Channel::getTopic() const {
	return _topic;
}

void Channel::setTopic(const std::string &topic) {
	_topic = topic;
}

