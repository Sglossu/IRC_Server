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
    _commands["AWAY"] = &Handler::_cmd_away;

	// channel
	_commands["JOIN"] = &Handler::_cmd_join;
	_commands["INVITE"] = &Handler::_cmd_invite;
	_commands["KICK"] = &Handler::_cmd_kick;
	_commands["MODE"] = &Handler::_cmd_mode;
	_commands["PART"] = &Handler::_cmd_part;
	_commands["TOPIC"] = &Handler::_cmd_topic;
	_commands["NAMES"] = &Handler::_cmd_names;

	// system
	_commands["WHO"] = &Handler::_cmd_who;
	_commands["ISON"] = &Handler::_cmd_ison;
	_commands["PING"] = &Handler::_cmd_ping_pong;

}

void	Handler::_registration_modes() {
	_modes['o'] = &Handler::_mode_o;
	_modes['p'] = &Handler::_mode_p;
	_modes['s'] = &Handler::_mode_s;
	_modes['i'] = &Handler::_mode_i;
	_modes['k'] = &Handler::_mode_k;
	_modes['b'] = &Handler::_mode_b;
}

