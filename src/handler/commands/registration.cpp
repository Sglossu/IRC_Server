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

//	if (user.getPass().empty())
//		_error_msg(&msg, &user, 462);
	if  (!msg.get_params().size())
		_error_msg(&msg, &user, 461);
	else if (msg.get_params()[0].compare(_server.getPass())) {
		std::cout << "<User: fd " << user.getFdSock() << "> password is incorrect" << std::endl;
		_error_msg(&msg, &user, 464);
	}
	else {
		std::cout << "<User: fd " << user.getFdSock() << "> password is correct" << std::endl;
		user.setEnterPass(true);
		user.setPass(msg.get_params()[0]);
		// todo проверить когда всё установлено что пароль верный
	}
}

void	Handler::_cmd_nick(Message &msg, User &user) {
	std::cout << "cmd_nick " << user.getUsername() << std::endl;

	if (user.isEnterNick())
		_error_msg(&msg, &user, 462);
	else if (!msg.get_params().size())
		_error_msg(&msg, &user, 431);
	else if (!is_nickname_correct(msg.get_params()[0]))
		_error_msg(&msg, &user, 432);
	else
	{
		for (std::map<int, User *>::iterator it = _server.map_users.begin(); it != _server.map_users.end(); it++)
		{
			if (!it->second->getNick().compare(msg.get_params()[0])) {
				_error_msg(&msg, &user, 433);
				return ;
			}
		}
		user.setNick(msg.get_params()[0]);
		user.setEnterNick(true);
		std::cout << "<User: fd " << user.getFdSock() << "> has nick " << user.getNick() <<std::endl;
	}
}

void	Handler::_cmd_user(Message &msg, User &user) {
	std::cout << "cmd_user " << user.getUsername() << std::endl;
	if (user.isEnterName()) {
		_error_msg(&msg, &user, 462);
		return ;
	}
	else if (msg.get_params().size() < 4) {
		_error_msg(&msg, &user, 461);
		return ;
	}
	user.setUsername(msg.get_params()[0]);
	user.setHostname(msg.get_params()[1]);
	user.setServername(msg.get_params()[2]);

	if (msg.get_params()[3][0] != ':')
		user.setRealname(msg.get_params()[3]);
	else {
		std::string realname = "";
		for (int i = 3; i < msg.get_params().size(); i++) {
			if (i != 3) {
				realname += " ";
			}
			realname += msg.get_params()[i];
		}
		user.setRealname(realname);
	}
	user.setEnterName(true);
	std::cout << "<User: fd " << user.getFdSock() << "> has username: " << user.getUsername() <<
			  ", hostname: " << user.getHostname() <<
			  ", servername: " << user.getServername() <<
			  ", realname: "<< user.getRealname() << std::endl;
}

void	Handler::_cmd_oper(Message &msg, User &user) {
	std::cout << "cmd_oper " << user.getUsername() << std::endl;

	if (msg.get_params().size() < 2) {
		_error_msg(&msg, &user, 461);
		return ;
	}
	for (std::map<int, User *>::iterator it = _server.map_users.begin(); it != _server.map_users.end(); it++)
	{
		if (it->second->getUsername().compare(msg.get_params()[0])) {
			_error_msg(&msg, &user, 464);
			return ;
		}
	}
	std::string 	chiper_pass = sha256(msg.get_params()[1]);
	std::cout << "sha256('"<< msg.get_params()[1] << "'): " << chiper_pass << std::endl;

	std::cout << "chiper_pass: " << chiper_pass << "config: " << getConfig()["operators." + user.getUsername()];

	if (getConfig()["opeartors." + user.getUsername()] != chiper_pass)
		_error_msg(&msg, &user, 464);
	else {
		_cmd_responses(&msg, &user, 381);
		user.setIrcOperator(true);
	}

}

void	Handler::_cmd_quit(Message &msg, User &user) {}