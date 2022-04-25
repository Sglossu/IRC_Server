#include "../Handler.hpp"

void	Handler::_registration_commands() {
	// registration
	_commands["PASS"] = &Handler::_cmd_pass;
	_commands["NICK"] = &Handler::_cmd_nick;
	_commands["USER"] = &Handler::_cmd_user;
	_commands["OPER"] = &Handler::_cmd_oper;
	_commands["QUIT"] = &Handler::_cmd_quit;

	// user_cmd
    _commands["PRIVMSG"] = &Handler::_cmd_privmsg;

	// channel
	_commands["JOIN"] = &Handler::_cmd_join;
	_commands["INVITE"] = &Handler::_cmd_invite;
	_commands["KICK"] = &Handler::_cmd_kick;
	_commands["MODE"] = &Handler::_cmd_mode;

	// system
	_commands["PING"] = &Handler::_cmd_ping_pong;



}

