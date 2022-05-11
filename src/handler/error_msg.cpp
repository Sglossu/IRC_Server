#include "Handler.hpp"

// error 5** - our solution

void	Handler::_error_msg(User &user, int er,const std::string &msg) {
	std::string	error_msg;
	error_msg = ":IrcServer " + numberToString(er) + " ";
	switch (er) {
		case 401: error_msg += user.getNick() + " " + msg + ":No such nick/channel"; break;
		case 403: error_msg += user.getNick() + " " + msg + " :No such channel"; break;
		case 404: error_msg += user.getNick() + " " + msg + " :Cannot send to channel"; break;
		case 405: error_msg += user.getNick() + " " + msg + " :You have joined too many channelsl"; break;
        case 411: error_msg += user.getNick() + " :No recipient given (<command>)"; break;
        case 412: error_msg += user.getNick() + " :No text to send"; break;
		case 421: error_msg += user.getNick() + " " + msg + " :Unknown command"; break;
		case 431: error_msg += user.getNick() + " :No nickname given"; break;
		case 432: error_msg += user.getNick() + " " + msg + " :Erroneus nickname"; break;
		case 433: error_msg += user.getNick() + " " + msg + " :Nickname is already in use"; break;
		case 442: error_msg += user.getNick() + " " + msg + " :You're not on that channel"; break;
		case 443: error_msg += user.getNick() + " " + msg + " :Is already on channel"; break;
		case 451: error_msg += user.getNick() + " :You have not registered"; break;
		case 461: error_msg += user.getNick() + " " + msg + " :Not enough parameters"; break;
		case 462: error_msg += user.getNick() + " :You may not reregister"; break;
		case 464: error_msg += user.getNick() + " :Password incorrect"; break;
		case 467: error_msg += user.getNick() + " " + msg + " :Channel key already set"; break;
		case 472: error_msg += user.getNick() + " " + msg + " :is unknown mode char to me"; break;
		case 473: error_msg += user.getNick() + " " + msg + " :Cannot join channel (+i)"; break;
		case 474: error_msg += user.getNick() + " " + msg + " :Cannot join channel (+b)"; break;
		case 475: error_msg += user.getNick() + " " + msg + " :Cannot join channel (+k)"; break;
		case 482: error_msg += user.getNick() + " " + msg + " :You're not channel operator"; break;
		case 501: error_msg += user.getNick() + " :Unknown MODE flag"; break;
		case 502: error_msg += user.getNick() + " :Cant change mode for other users"; break;
		case 1001: error_msg  = ":IrcServer ERROR " + msg; break;
	}
	error_msg += CR_LF;
	_server.write_to_client(user.getFdSock(), error_msg);
}



void Handler::_cmd_responses(std::string msg, User &user, int er) {
	std::string	resp_msg;
	resp_msg = ":IrcServer " + numberToString(er) + " ";
	switch (er) {
		case 221: resp_msg += msg; break;
		case 301: resp_msg += msg; break;
		case 303: resp_msg += msg; break;
		case 305: resp_msg += msg + " :You are no longer marked as being away"; break;
		case 306: resp_msg += msg + " :You have been marked as being away"; break;
		case 315: resp_msg += msg + " :End of WHO list"; break;
		case 331: resp_msg += msg + " :No topic is set"; break;
		case 332: resp_msg += msg; break; // сюда придет полный msg с сообщением
		case 341: resp_msg += msg; break; // сюда придет полный msg с сообщением
		case 352: resp_msg += msg; break; // сюда придет полный msg с сообщением
		case 353: resp_msg += msg; break; // сюда придет полный msg с сообщением
		case 366: resp_msg += msg + " :End of /NAMES list"; break;
		case 381: resp_msg += msg + " :You are now an IRC operator"; break;
	}
	resp_msg += CR_LF;
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

