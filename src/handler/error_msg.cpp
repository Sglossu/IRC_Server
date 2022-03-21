#include "Handler.hpp"

void	Handler::_error_msg(Message *msg, User *user, int er) {
	std::string	error_msg;
	switch (er) {
		case 461:
			error_msg = "Error 461 :Not enough parameters\r\n";
			break;
		case 433:
			error_msg = "Error 433 :Nickname is already in use\r\n";
			break;
		case 462:
			error_msg = "Error 462 :You may not reregister\r\n";
			break;
		case 431:
			error_msg = "Error 431 :No nickname given\r\n";
			break;
		case 432:
			error_msg = "Error 432 :Erroneus nickname\r\n";
			break;
	}
	_server.write_to_client(user->getFdSock(), error_msg);
}