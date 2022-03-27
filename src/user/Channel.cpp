#include "Channel.hpp"
#include "../handler/Handler.hpp"

Channel::Channel(std::string name, User &user, std::string pass, Handler *handler) {
	_name = name;
	_pass = pass;
	_handler = handler;
	_join_user(user, pass);
}

Channel::~Channel() {
	//todo: удаляем всех пользователей с группы через KICK?
}


void Channel::_delete_user(std::string &username) {
	std::vector<std::string>::iterator it;

	it = std::find(begin(_users), end(_users), username);
	if (it != _users.end()) {
		// todo: send user left the channel
		_users.erase(it);
	}

}

/*
		  1.  Пользователь может быть приглашен, если канал invite-only;
          2.  Никнейм/имя пользователя/имя хоста не должны быть
              забанеными;
          3.  Если установлен пароль, но должен быть верным.
*/
void	Channel::_join_user(User &user, std::string pass) {
	// todo check if invite-only
	// todo ban

	// check pass
	if (_pass.compare(pass))
		_handler->_error_msg(user, 475);
	else {
		_users.push_back(user.getUsername());
		_return_topic(user);
	}
}

void Channel::_return_topic(User &user) {
	_handler->_cmd_responses(_name, user, 331);

	std::string names = "= " + _name + " :";
	for (int i = 0; i < _users.size(); i++) {
		if (i == 0)
			names += "@" + _users[i];
		else
			names += " " + _users[i];
	}
	_handler->_cmd_responses(names, user, 353);
	_handler->_cmd_responses(_name, user, 366);
	return ;
}

const std::string &Channel::getName() const {
	return _name;
}

void Channel::setName(const std::string &name) {
	_name = name;
}

