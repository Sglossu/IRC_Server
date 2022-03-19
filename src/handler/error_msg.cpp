#include "Handler.hpp"

void	Handler::_error_msg(Message *msg, User *user, int er) {
	std::string	error_msg;
	switch (er) {
		case 461:
			error_msg = msg->get_cmd() + " :Bad parameters\r\n"; // original : Not enough parameters
			break;
		case 433:
			error_msg = msg->get_cmd() + " :Nickname is already in use\r\n"; // original : Not enough parameters
			break;
	}
	_server.write_to_client(user->getFdSock(), error_msg);
}