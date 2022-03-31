#include "../Handler.hpp"

const std::string Handler::form_privmsg(const Message &raw_msg, const User &sender) {
    std::string privmsg;

    privmsg = ":" + sender.getNick() + "!" + sender.getUsername() + "@" + _server.ipstr[1] + " PRIVMSG ";
    // if (raw_msg.get_params()[1].empty()) 
    //     privmsg += ":" + sender.getNick(); 
    // else 
    // под комментом реализация как у Макса с Эриком
        privmsg += sender.getNick() + " :" + raw_msg.get_params()[1];
    privmsg += "\r\n";
    return privmsg;
}

void    Handler::_cmd_privmsg(Message &msg, User &user) {
    std::cout << "cmd_privmsg " << user.getNick() << std::endl;

    if (!msg.get_params().size())
		return _error_msg(user, 411);
    if (msg.get_params().size() < 2)
        return _error_msg(user, 412);
    std::string         privmsg = form_privmsg(msg, user);
    std::string         receiver_nick;
    std::istringstream  receivers_stream(msg.get_params()[0]);
    std::map<std::string, User *>::iterator it;
    while (getline(receivers_stream, receiver_nick, ',')) {
        std::cout << receiver_nick << std::endl;
        it = _server.mapnick_users.find(receiver_nick);
        if (it != _server.mapnick_users.end())
            _server.write_to_client(it->second->getFdSock(), privmsg);
    }
}