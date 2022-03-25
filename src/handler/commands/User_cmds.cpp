#include "../Handler.hpp"

void    Handler::_cmd_privmsg(Message &msg, User &user) {
    std::cout << "cmd_privmsg" << user.getUsername() << std::endl;

    
}