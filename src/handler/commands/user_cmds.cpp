#include "../Handler.hpp"

const std::string Handler::_form_privmsg(const Message &raw_msg, const User &sender, std::string &receiver_nick) {
    std::string privmsg;

    privmsg = prefix_msg(sender);
    privmsg += "PRIVMSG " + receiver_nick + " :" + raw_msg.get_params()[1];
    privmsg += CR_LF;
    return privmsg;
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
        if (receiver_nick[0] == '@' or receiver_nick[0] == '#') {
            // todo add send to channel
        }
        else 
            _server.write_to_client(receiver_nick, privmsg);
    }
}