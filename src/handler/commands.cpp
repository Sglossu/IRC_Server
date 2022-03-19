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
	std::cout << "cmd_pass " << user.get_name() << std::endl;
	// todo

	if (!msg.get_params().size() || msg.get_params()[0].compare(_server.getPass()))
		_error_msg(&msg, &user, 461);
	else
		std::cout << "<User: fd " << user.getFdSock() << "> password is correct" << std::endl;
}

void	Handler::_cmd_nick(Message &msg, User &user) {
	std::cout << "cmd_nick " << user.get_name() << std::endl;
	// todo

}

void	Handler::_cmd_user(Message &msg, User &user) {
	std::cout << "cmd_user " << user.get_name() << std::endl;
	// todo

}