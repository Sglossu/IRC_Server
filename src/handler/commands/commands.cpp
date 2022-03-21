#include "../Handler.hpp"

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

