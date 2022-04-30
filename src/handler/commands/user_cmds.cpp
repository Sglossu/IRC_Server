#include "../Handler.hpp"

const std::string Handler::_form_privmsg(const Message &raw_msg, const User &sender, std::string &receiver_nick) {
    std::string privmsg;

    privmsg = ":" + sender.getNick() + "!" + sender.getUsername() + "@" + _server.ipstr[0] + " PRIVMSG "; // ipstr[1] isn't corerrect
    // if (raw_msg.get_params()[1].empty()) 
    //     privmsg += ":" + sender.getNick(); 
    // else 
    // под комментом реализация как у Макса с Эриком
        privmsg += receiver_nick + " :" + raw_msg.get_params()[1];
    privmsg += CR_LF;
    return privmsg;
}

void    Handler::_cmd_privmsg(Message &msg, User &user) {
    std::cout << "cmd_privmsg " << user.getNick() << std::endl;

    if (!msg.get_params().size())
		return _error_msg(user, 411);
    if (msg.get_params().size() < 2)
        return _error_msg(user, 412);
    
    std::string         receiver_nick;
    std::istringstream  receivers_stream(msg.get_params()[0]);
    std::map<std::string, User *>::iterator it;
    while (getline(receivers_stream, receiver_nick, ',')) {
        if (!receiver_nick.size())
            continue;
        // std::cout << receiver_nick << std::endl;
        std::string privmsg = _form_privmsg(msg, user, receiver_nick);
        std::cout << "formed msg to send| " << privmsg;
        if (receiver_nick[0] == '@' or receiver_nick[0] == '#') {
            // todo add send to channel
        }
        else {
            // it = _server.mapnick_users.find(receiver_nick);
            // if (it != _server.mapnick_users.end()) {
                // std::cout << "send prv msg from" << user.getNick() << " to " << it->second->getFdSock() << std::endl;
            _server.write_to_client(receiver_nick, privmsg); // хотел сделать отправку по нику но хуй
            // }
        }
    }
}