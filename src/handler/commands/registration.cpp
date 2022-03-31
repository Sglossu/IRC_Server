#include "../Handler.hpp"

bool	is_nickname_correct(std::string nick) {
	if (!std::isalpha(nick[0]))
		return false;
	for (int i = 1; i < nick.size(); i++) {
		if (!std::isalpha(nick[i]) && !std::isdigit(nick[i]))
			return false;
	}
	return true;
}

void	Handler::_cmd_pass(Message &msg, User &user) {
	std::cout << "cmd_pass " << user.getUsername() << std::endl;
	std::cout << "Incoming pass: " << msg.get_params()[0] << " size (" << msg.get_params()[0].size() << ")" << std::endl;
	if  (!msg.get_params().size())
		_error_msg(user, 461);
	else if (msg.get_params()[0].compare(_server.getPass())) {
		std::cout << "<User: fd " << user.getFdSock() << "> password is incorrect" << std::endl;
		_error_msg(user, 464);
	}
	else {
		std::cout << "<User: fd " << user.getFdSock() << "> password is correct" << std::endl;
		user.setPass(msg.get_params()[0]);
		user.set_flag(ENTER_PASS);
	}
}

void	Handler::_cmd_nick(Message &msg, User &user) {
	std::cout << "cmd_nick " << user.getUsername() << std::endl;

	if (!(user.get_flags() & ENTER_NICK) && !msg.get_prefix().empty())
		_error_msg(user, 462);
	else if (!msg.get_params().size())
		_error_msg(user, 431);
	else if (!is_nickname_correct(msg.get_params()[0]))
		_error_msg(user, 432);
	else
	{
		for (std::map<int, User *>::iterator it = _server.mapfd_users.begin(); it != _server.mapfd_users.end(); it++)
		{
			if (!it->second->getNick().compare(msg.get_params()[0])) {
				_error_msg(user, 433);
				return ;
			}
		}
		user.setNick(msg.get_params()[0]);
		user.set_flag(ENTER_NICK);
		std::cout << "<User: fd " << user.getFdSock() << "> has nick {" << user.getNick() << "}" <<std::endl;
	}
}

void	Handler::_cmd_user(Message &msg, User &user) {
	std::cout << "cmd_user " << user.getUsername() << std::endl;
	if (user.get_flags() & ENTER_NAME) {
		_error_msg(user, 462);
		return ;
	}
	else if (msg.get_params().size() < 4) {
		_error_msg(user, 461);
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
		_error_msg(user, 461);
		return ;
	}
	for (std::map<int, User *>::iterator it = _server.mapfd_users.begin(); it != _server.mapfd_users.end(); it++)
	{
		if (it->second->getUsername().compare(msg.get_params()[0])) {
			_error_msg(user, 464);
			return ;
		}
	}
	std::string 	chiper_pass = sha256(msg.get_params()[1]);
	std::string ret_config = getConfig()["operators." + user.getUsername()];

	if (chiper_pass.compare(ret_config))
		_error_msg(user, 464);
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