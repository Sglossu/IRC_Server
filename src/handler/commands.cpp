#include "Handler.hpp"

void	Handler::_registration_commands() {
	_commands["PASS"] = &Handler::_cmd_pass;
	_commands["NICK"] = &Handler::_cmd_nick;
	_commands["USER"] = &Handler::_cmd_user;


	_commands["PIZDATIY_TEST1"] = &Handler::_test1;
}

void	Handler::_test1(Message &msg, User &user) {
	std::cout << "------------PIZDATIY_TEST1-------------" << std::endl;
	Message m_pass("PASS " + _server.pass);
	Message m_nick("NICK Nagibator666");
	Message m_user("User bla bla Oleg");

	_cmd_pass(m_pass, user);
	_cmd_nick(m_nick, user);
	_cmd_user(m_user, user);

}

void	Handler::_cmd_pass(Message &msg, User &user) {
	std::cout << "cmd_pass " << user.getUsername() << std::endl;

	if (!msg.get_params().size() || msg.get_params()[0].compare(_server.getPass()))
		_error_msg(&msg, &user, 461);
	else
		std::cout << "<User: fd " << user.getFdSock() << "> password is correct" << std::endl;
}

void	Handler::_cmd_nick(Message &msg, User &user) {
	std::cout << "cmd_nick " << user.getUsername() << std::endl;
	if (!msg.get_params().size()) {
		_error_msg(&msg, &user, 461);
		return ;
	}

//	std::string		nick = msg.get_params()[0];
	for (std::map<int, User *>::iterator it = _server.map_users.begin(); it != _server.map_users.end(); it++)
	{
		if (!it->second->getUsername().compare(msg.get_params()[0])) {
			_error_msg(&msg, &user, 433);
			return ;
		}
	}
	user.setNick(msg.get_params()[0]);
	std::cout << "<User: fd " << user.getFdSock() << "> has nick " << user.getNick() <<std::endl;
}

void	Handler::_cmd_user(Message &msg, User &user) {
	std::cout << "cmd_user " << user.getUsername() << std::endl;
	if (!user.getUsername().empty() && !user.getHostname().empty() &&
		!user.getServername().empty() && !user.getRealname().empty()) {
			_error_msg(&msg, &user, 462);
			return ;
	}
	if (msg.get_params().size() < 4) {
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
	std::cout << "<User: fd " << user.getFdSock() << "> has username: " << user.getUsername() <<
						", hostname:" << user.getHostname() <<
						", servername:" << user.getServername() <<
						", realname: "<< user.getRealname() << std::endl;
}