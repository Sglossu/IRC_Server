#include "Handler.hpp"

// error 5** - our solution

void	Handler::_error_msg(User &user, int er,const std::string &msg) {
	std::string	error_msg;
	error_msg = ":IrcServer " + numberToString(er) + " ";
	switch (er) {
		case 401: error_msg += user.getNick() + " " + msg + ":No such nick/channel\r\n"; break;
		case 403: error_msg += user.getNick() + " " + msg + " :No such channel\r\n"; break;
		case 404: error_msg += user.getNick() + " " + msg + " :Cannot send to channel\r\n"; break;
		case 405: error_msg += user.getUsername() + " :You have joined too many channelsl\r\n"; break;
        case 411: error_msg += user.getNick() + " :No recipient given (<command>)\r\n"; break;
        case 412: error_msg += user.getNick() + " :No text to send\r\n"; break;
		case 421: error_msg += user.getUsername() + " :Unknown command\r\n"; break;
		case 431: error_msg += user.getUsername() + " :No nickname given\r\n"; break;
		case 432: error_msg += user.getUsername() + " :Erroneus nickname\r\n"; break;
		case 433: error_msg += user.getUsername() + " :Nickname is already in use\r\n"; break;
		case 442: error_msg += user.getUsername() + " :You're not on that channel\r\n"; break;
		case 443: error_msg += user.getNick() + " " + msg + " :Is already on channel\r\n"; break;
		case 451: error_msg += user.getUsername() + " :You have not registered\r\n"; break;
		case 461: error_msg += user.getUsername() + " :Not enough parameters\r\n"; break;
		case 462: error_msg += user.getUsername() + " :You may not reregister\r\n"; break;
		case 464: error_msg += user.getUsername() + " :Password incorrect\r\n"; break;
		case 467: error_msg += user.getUsername() + " :Channel key already set\r\n"; break;
		case 473: error_msg += user.getUsername() + " :Cannot join channel (+i)\r\n"; break;
		case 472: error_msg += user.getUsername() + " :is unknown mode char to me\r\n"; break;
		case 474: error_msg += user.getUsername() + " :Cannot join channel (+b)\r\n"; break;
		case 475: error_msg += user.getUsername() + " :Cannot join channel (+k)\r\n"; break;
		case 482: error_msg += user.getUsername() + " :You're not channel operator\r\n"; break;
		case 501: error_msg += user.getUsername() + " :Unknown MODE flag\r\n"; break;
		case 502: error_msg += user.getUsername() + " :Password is not entered\r\n"; break;
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

