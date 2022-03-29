#include "../Handler.hpp"

void    Handler::_cmd_privmsg(Message &msg, User &user) {
    std::cout << "cmd_privmsg " << user.getUsername() << std::endl;

    if (!msg.get_params().size())
		  _error_msg(&msg, &user, 461);
    std::istringstream  receivers(msg.get_params()[0]);
    std::string         receiver_nick;
    std::map<UserKey, User *>::iterator it;
    while (getline(receivers, receiver_nick, ',')) {
        std::cout << receiver_nick << std::endl;
        //it = _server.map_users.find(receiver_nick);
       // if (it != _server.map_users.end())
            
    }
}