#include "../Handler.hpp"

const std::string Handler::_form_privmsg(const Message &raw_msg, const User &sender, std::string &receiver_nick) {
    std::string privmsg;

    privmsg = prefix_msg(sender);
    privmsg += "PRIVMSG " + receiver_nick + " :" + raw_msg.get_params()[1];
    privmsg += CR_LF;
    return privmsg;
}

void    Handler::_cmd_privmsg_channel(User &user, const std::string &name, const std::string &msg) {
    Channel *channel = _server.map_channels[name];

    if (!channel)
        return _error_msg(user, 401, name);
    if (channel->_is_user_in_banlist(user.getNick()))
        return _error_msg(user, 404, name);
    _write_to_channel(*channel, msg, user.getNick());
}

void    Handler::_cmd_privmsg_user(User &user, const std::string &name, const std::string &msg) {
    User *recv = _server.mapnick_users[name];
    if (!recv)
        return _error_msg(user, 401, name);
    if (recv->getRplAway().size()) {
        std::string awayMsg;
        awayMsg = user.getNick() + " " + recv->getNick() + " " + recv->getRplAway();
        return _cmd_responses(awayMsg, user, 301);
    }
    // todo можно добавить чек на "черный  список"
    _server.write_to_client(name, msg);
}

void    Handler::_cmd_privmsg(Message &msg, User &user) {
    std::cout << "cmd_privmsg " << user.getNick() << std::endl;

    if (!msg.get_params().size())
		return _error_msg(user, 411, "");
    if (msg.get_params().size() < 2)
        return _error_msg(user, 412, "");
    
    std::string         receiver_nick;
    std::istringstream  receivers_stream(msg.get_params()[0]);
    while (getline(receivers_stream, receiver_nick, ',')) {
        if (!receiver_nick.size())
            continue;
        std::string privmsg = _form_privmsg(msg, user, receiver_nick);
        std::cout << "formed msg to send| " << privmsg;
        if (receiver_nick[0] == '@' or receiver_nick[0] == '#')
            _cmd_privmsg_channel(user, receiver_nick, privmsg);
        else
            _cmd_privmsg_user(user, receiver_nick, privmsg);
    }
}

void	Handler::_cmd_away(Message &msg, User &user) {
	std::cout << "cmd_away " << user.getNick() << std::endl;
	if (msg.get_params().size() < 1 or msg.get_params()[0].empty()) {
        user.setRplAway("");
		return _cmd_responses(user.getNick(), user, 305);
    }
	user.setRplAway(msg.get_params()[0]);
	return _cmd_responses(user.getNick(), user, 306);
}

