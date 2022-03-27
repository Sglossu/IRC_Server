#pragma once
#include "../../inc/irc.hpp"
#include "User.hpp"
//#include "../handler/Handler.hpp"

class Handler;

class Channel {
private:
	std::string					_name;
	std::vector<std::string>	_users;
	std::string 				_pass;
	Handler						*_handler;
public:
	Channel(std::string name, User &user, std::string pass, Handler *handler);
	~Channel();

	void				_delete_user(std::string &username);
	void				_join_user(User *user, std::string pass);
	void 				_topic();

	const std::string	&getName() const;
	void				setName(const std::string &name);
};
