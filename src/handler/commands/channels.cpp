#include "../Handler.hpp"

void	Handler::_cmd_join(Message &msg, User &user) {
	std::cout << "cmd_join " << user.getUsername() << std::endl;

//	_server.getMapChannels()[user.getFdSock()] = new Channel(, user);
}