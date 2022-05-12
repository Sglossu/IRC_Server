#include "../Handler.hpp"

void    Handler::_cmd_ping(Message &msg, User &user) {
	if (DEBUG > 1)
    	std::cout << "ping msg" << user.getNick() << std::endl;

    if (!msg.get_params().size())
		_error_msg(user, 461, "PING");
	else {
		std::string privmsg = "PONG :" + msg.get_params()[0] + CR_LF;
	    _server.write_to_client(user.getFdSock(), privmsg);
	}
}

void    Handler::_cmd_pong(Message &msg, User &user) {
	if (DEBUG > 1)
    	std::cout << "pong msg" << user.getNick() << std::endl;

    if (!msg.get_params().size())
		_error_msg(user, 461, "PONG");
	// else {
	// 	std::string privmsg = "PING :" + msg.get_params()[0] + CR_LF;
	//     _server.write_to_client(user.getFdSock(), privmsg);
	// }
}

void    Handler::_cmd_ison(Message &msg, User &user) {
	if (DEBUG > 1)
    	std::cout << "ISON msg" << user.getNick() << std::endl;

    if (!msg.get_params().size())
		_error_msg(user, 461, "ISON");
	else {
		std::string available_nicks = user.getNick() + " :";
		for (size_t i = 0; i < msg.get_params().size(); ++i) {
			std::string nick = msg.get_params()[i];
			if (_server.mapnick_users.count(nick) and !(_server.mapnick_users[nick]->get_flags() & DISCONNECTED)) {
				if (available_nicks[available_nicks.size() - 1] == ':')
					available_nicks += nick;
				else
					available_nicks += " " + nick;
			}
		}
	    _cmd_responses(available_nicks, user, 303);
	}
}


void    Handler::_cmd_who(Message &msg, User &user) {
	if (DEBUG)
    	std::cout << "who msg" << user.getNick() << std::endl;

    // work only with channelname
	if (!msg.get_params().size() or !msg.get_params()[0].size())
		return _cmd_responses(" ", user, 315);;
	std::string mask = msg.get_params()[0];
	if (_server.map_channels.count(mask)) {
		Channel *channel = _server.map_channels[mask];
		std::string prefix, host_part;
		prefix = user.getNick() + " " + mask + " ~";
		host_part = " " + _host + " " + _host + " ";
		std::set<std::string> opers(channel->getOperators().begin(), channel->getOperators().end());
		for (size_t i = 0; i < channel->getUsers().size() and i < 256; ++i) {
			std::string nick = channel->getUsers()[i];
			if (!_server.mapnick_users.count(nick))
				continue;
			User *user_to_fill = _server.mapnick_users[nick];
			std::string username = user_to_fill->getUsername();
			std::string realname = user_to_fill->getRealname();
			std::string flags;
			flags = user_to_fill->getRplAway().size() ? " G" : " H";
			if (opers.count(nick))
				flags += "*";
			std::string msg = prefix + username + host_part + nick + flags + " :1 " + realname;
			_cmd_responses(msg, user, 352);
		}
	}
	_cmd_responses(mask, user, 315);
}

