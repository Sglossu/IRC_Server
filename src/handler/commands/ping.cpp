#include "../Handler.hpp"

void    Handler::_cmd_ping_pong(Message &msg, User &user) {
    std::cout << "ping_pong msg" << user.getNick() << std::endl;

    std::string privmsg = "PONG";
    if (msg.get_params().size())
        privmsg += " :" + msg.get_params()[0];
    privmsg += CR_LF;
    
    _server.write_to_client(user.getFdSock(), privmsg);
}
