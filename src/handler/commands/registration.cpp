#include "../Handler.hpp"

bool	is_nickname_correct(std::string nick) {
	if (!std::isalpha(nick[0]))
		return false;
	for (size_t i = 1; i < nick.size(); i++) {
		if (!std::isalpha(nick[i]) && !std::isdigit(nick[i]))
			return false;
	}
	return true;
}

void	Handler::_cmd_pass(Message &msg, User &user) {
	std::cout << "cmd_pass " << user.getUsername() << std::endl;
	std::cout << "Incoming pass: " << msg.get_params()[0] << " size (" << msg.get_params()[0].size() << ")" << std::endl;
	if  (!msg.get_params().size())
		_error_msg(user, 461, "PASS");
	else if (msg.get_params()[0].compare(_server.getPass())) {
		std::cout << "<User: fd " << user.getFdSock() << "> password is incorrect" << std::endl;
		_error_msg(user, 464, "");
	}
	else {
		std::cout << "<User: fd " << user.getFdSock() << "> password is correct" << std::endl;
		user.setPass(msg.get_params()[0]);
		user.set_flag(ENTER_PASS);
	}
}

void	Handler::_change_old_nick(User &user, std::string new_nick) {
	// шлем всем сообщение о смене ника
	std::string change_nick_msg = prefix_msg(user) + "NICK " + new_nick + CR_LF;
	_server.broadcast_message(change_nick_msg);

	_server.mapnick_users.erase(user.getNick());
	_server.mapnick_users[new_nick] = &user;

	// change nick to new in all user channels
	const std::vector<std::string> uchannels = user.getChanels();
	for (size_t i = 0; i < uchannels.size(); ++i){

		if (!_server.map_channels.count(uchannels[i]))
			continue;
		
		Channel *channel = _server.map_channels[uchannels[i]];
		channel->changeNick(user.getNick(), new_nick);
	}
}

void	Handler::_cmd_nick(Message &msg, User &user) {
	std::cout << "cmd_nick " << user.getUsername() << std::endl;

	if (!(user.get_flags() & ENTER_NICK) && !msg.get_prefix().empty())
		return _error_msg(user, 462, "");
	if (!msg.get_params().size())
		return _error_msg(user, 431, "");
	std::string new_nick = msg.get_params()[0];
	if (!is_nickname_correct(new_nick))
		return _error_msg(user, 432, new_nick);

	for (std::map<int, User *>::iterator it = _server.mapfd_users.begin(); it != _server.mapfd_users.end(); it++)
		if (!it->second->getNick().compare(new_nick))
			return _error_msg(user, 433, new_nick);
	// если это смена ника, то меняем в мапе, в группах, и оповещаем всех пользователей
	if (user.get_flags() & ENTER_NICK) 
		_change_old_nick(user, new_nick);
	user.setNick(new_nick);
	user.set_flag(ENTER_NICK);
	std::cout << "<User: fd " << user.getFdSock() << "> has nick {" << user.getNick() << "}" <<std::endl;
}

void	Handler::_cmd_user(Message &msg, User &user) {
	std::cout << "cmd_user " << user.getUsername() << std::endl;
	if (user.get_flags() & ENTER_NAME) {
		_error_msg(user, 462, "");
		return ;
	}
	else if (msg.get_params().size() < 4) {
		_error_msg(user, 461, "USER");
		return ;
	}
	user.setUsername(msg.get_params()[0]);
	user.setHostname(msg.get_params()[1]);
	user.setServername(msg.get_params()[2]);
	user.setRealname(msg.get_params()[3]);
	user.set_flag(ENTER_NAME);
	std::cout << "<User: fd " << user.getFdSock() << "> has username: {" << user.getUsername() <<
			  "}, hostname: {" << user.getHostname() <<
			  "}, servername: {" << user.getServername() <<
			  "}, realname: {"<< user.getRealname() << "}" << std::endl;
}

void	Handler::_cmd_oper(Message &msg, User &user) {
	std::cout << "cmd_oper " << user.getUsername() << std::endl;

	if (msg.get_params().size() < 2) {
		_error_msg(user, 461, "OPER");
		return ;
	}
	for (std::map<int, User *>::iterator it = _server.mapfd_users.begin(); it != _server.mapfd_users.end(); it++)
	{
		if (it->second->getUsername().compare(msg.get_params()[0])) {
			_error_msg(user, 464, "");
			return ;
		}
	}
	std::string 	chiper_pass = sha256(msg.get_params()[1]);
	std::string ret_config = getConfig()["operators." + user.getUsername()];

	if (chiper_pass.compare(ret_config))
		_error_msg(user, 464, "");
	else {
		_cmd_responses("" , user, 381);
		user.set_flag(IRC_OPERATOR);
	}

}

void	Handler::_cmd_quit(Message &msg, User &user) {
	std::cout << "cmd_quit " << user.getUsername() << std::endl;

	_server.mapnick_users.erase(user.getNick());
	// остальное очистится на стороне сервера
	// todo удалить пользователя из канала (и проверить, что quit-сообщение туда отправилось)

	user.set_flag(DISCONNECTED);
}