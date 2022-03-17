#pragma once
#include "../../inc/irc.hpp"
#include "User.hpp"




class Group {
private:
	std::string					_name;
	std::vector<std::string>	_users;

public:
	Group(std::string name, std::string &owner);
	~Group();

	void delete_user(std::string &username);
	const std::vector<std::string> &get_users() const;

};
