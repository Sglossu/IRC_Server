#pragma once
#include "../../inc/irc.hpp"

class User {
private:
	int				_fd_sock;
	std::string		_username;
	std::string		_realname;
	std::string		_nick;
	std::string		_message;
//	std::vector<>	_chanels; // придумать какой тип данных. массив для каналов юзера
public:
	User() {};
	User(int fd_sock);
	User(const User &other);
	User& operator=(const User& other);
	~User();
};

