#include "Handler.hpp"

// error 5** - our solution

void	Handler::_error_msg(Message *msg, User *user, int er) {
	std::string	error_msg;
	switch (er) {
		case 421:
			error_msg = "Error 421 :Unknown command\r\n";
			break;
		case 431:
			error_msg = "Error 431 :No nickname given\r\n";
			break;
		case 432:
			error_msg = "Error 432 :Erroneus nickname\r\n";
			break;
		case 433:
			error_msg = "Error 433 :Nickname is already in use\r\n";
			break;
		case 451:
			error_msg = "Error 451 :You have not registered\r\n";
			break;
		case 461:
			error_msg = "Error 461 :Not enough parameters\r\n";
			break;
		case 462:
			error_msg = "Error 462 :You may not reregister\r\n";
			break;
		case 464:
			error_msg = "Error 464 :Password incorrect\r\n";
			break;
		case 502:
			error_msg = "Error 502 :First, enter the password\r\n";
			break;

	}
	_server.write_to_client(user->getFdSock(), error_msg);
}

void Handler::_cmd_responses(Message *msg, User *user, int er) {
	std::string	resp_msg;
	switch (er) {
		case 381:
			resp_msg = "Response 381: You are now an IRC operator\r\n";
			break;
	}
	_server.write_to_client(user->getFdSock(), resp_msg);
}

