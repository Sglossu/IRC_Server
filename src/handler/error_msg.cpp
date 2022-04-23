#include "Handler.hpp"

// error 5** - our solution

void	Handler::_error_msg(User &user, int er) {
	std::string	error_msg;
	switch (er) {
		case 401: error_msg = ":IrcServer 401 " + user.getUsername() + " :No such nick/channel\r\n"; break;
		case 403: error_msg = ":IrcServer 403 " + user.getUsername() + " :No such channel\r\n"; break;
		case 405: error_msg = ":IrcServer 405 " + user.getUsername() + " :You have joined too many channelsl\r\n"; break;
        case 411: error_msg = ":IrcServer 411 " + user.getNick() + " :No recipient given (<command>)\r\n"; break;
        case 412: error_msg = ":IrcServer 412 " + user.getNick() + " :No text to send\r\n"; break;
		case 421: error_msg = ":IrcServer 421 " + user.getUsername() + " :Unknown command\r\n"; break;
		case 431: error_msg = ":IrcServer 431 " + user.getUsername() + " :No nickname given\r\n"; break;
		case 432: error_msg = ":IrcServer 432 " + user.getUsername() + " :Erroneus nickname\r\n"; break;
		case 433: error_msg = ":IrcServer 433 " + user.getUsername() + " :Nickname is already in use\r\n"; break;
		case 442: error_msg = ":IrcServer 442 " + user.getUsername() + " :You're not on that channel\r\n"; break;
		case 443: error_msg = ":IrcServer 443 " + user.getUsername() + " :Is already on channel\r\n"; break;
		case 451: error_msg = ":IrcServer 451 " + user.getUsername() + " :You have not registered\r\n"; break;
		case 461: error_msg = ":IrcServer 461 " + user.getUsername() + " :Not enough parameters\r\n"; break;
		case 462: error_msg = ":IrcServer 462 " + user.getUsername() + " :You may not reregister\r\n"; break;
		case 464: error_msg = ":IrcServer 464 " + user.getUsername() + " :Password incorrect\r\n"; break;
		case 473: error_msg = ":IrcServer 473 " + user.getUsername() + " :Cannot join channel (+i)\r\n"; break;
		case 475: error_msg = ":IrcServer 475 " + user.getUsername() + " :Cannot join channel (+k)\r\n"; break;
		case 482: error_msg = ":IrcServer 482 " + user.getUsername() + " :You're not channel operator\r\n"; break;
		case 502: error_msg = ":IrcServer 502 " + user.getUsername() + " :Password is not entered\r\n"; break;
	}
	_server.write_to_client(user.getFdSock(), error_msg);
}

void Handler::_cmd_responses(std::string msg, User &user, int er) {
	std::string	resp_msg;
	switch (er) {
		case 331: resp_msg = ":IrcServer 331 " + msg + " :No topic is set\r\n"; break;
		case 332: resp_msg = ":IrcServer 332 " + msg + "\r\n"; break; // сюда придет полный msg с сообщением
		case 341: resp_msg = ":IrcServer 341 " + msg + "\r\n"; break; // сюда придет полный msg с сообщением
		case 353: resp_msg = ":IrcServer 353 " + msg + "\r\n"; break; // сюда придет полный msg с сообщением
		case 366: resp_msg = ":IrcServer 366 " + msg + " :End of /NAMES list\r\n"; break;
		case 381: resp_msg = ":IrcServer 381 " + msg + " :You are now an IRC operator\r\n"; break;
	}
	_server.write_to_client(user.getFdSock(), resp_msg);
}

void Handler::_cmd_msgoftheday(User &user) {
	std::string motd = "IrcServer Message of the day - \r\n";

	std::string first = ":IrcServer 372 " + user.getNick() + " :";
	_server.write_to_client(user.getFdSock(), first + motd);
	_server.write_to_client(user.getFdSock(), first + "⋱ ⋮ ⋰\r\n");
	_server.write_to_client(user.getFdSock(), first + "⋯ ◯ ⋯ ︵ 　　　　　　^v^\r\n");
	_server.write_to_client(user.getFdSock(), first + "¸︵︵( ░░ )︵.︵.︵　　\r\n");
	_server.write_to_client(user.getFdSock(), first + "(´░░░░░░ ') ░░░' )\r\n");
	_server.write_to_client(user.getFdSock(), first + "`´︶´¯`︶´`︶´︶´`　^v^　　^v^\r\n");
	_server.write_to_client(user.getFdSock(), first + "╔┓┏╦━━╦┓╔┓╔━━╗╔╗\r\n");
	_server.write_to_client(user.getFdSock(), first + "║┗┛║┗━╣┃║┃║╯╰║║║\r\n");
	_server.write_to_client(user.getFdSock(), first + "║┏┓║┏━╣┗╣┗╣╰╯║╠╣\r\n");
	_server.write_to_client(user.getFdSock(), first + "╚┛┗╩━━╩━╩━╩━━╝╚╝\r\n");
	_server.write_to_client(user.getFdSock(), first + "♪♫•*¨*•.¸¸❤¸¸.•*¨*•♫♪\r\n");

	motd = ":IrcServer 376 " + user.getNick() + " :End of /MOTD" + CR_LF;
	_server.write_to_client(user.getFdSock(), motd);

}
