#pragma once
#include "../../inc/irc.hpp"
#include "User.hpp"

class Сhannel {
private:
	std::string					_name;
	std::vector<std::string>	_users;

public:
	Сhannel(std::string name, std::string &owner);
	~Сhannel();

	void delete_user(std::string &username);
	const std::vector<std::string> &get_users() const;
	

};
