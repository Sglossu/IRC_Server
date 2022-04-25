#pragma once
#include "../../inc/irc.hpp"
#include "User.hpp"
#include "algorithm"
//#include "../handler/Handler.hpp"

#define HAS_PASS		0b10000000
#define INVITE_ONLY		0b01000000

class Handler;

class Channel {
private:
	std::string					_name;
	std::vector<std::string>	_users;
	std::vector<std::string>	_operators;
	std::string 				_pass;
	std::string 				_topic;
	unsigned char				_flags;
	Handler						*_handler;
public:
	Channel(std::string name, User &user, std::string pass, Handler *handler);
	~Channel();

	void				_delete_user(std::string &username);
	void				_join_user(User &user, std::string pass, bool after_invite);
	bool				_is_user_on_channel(std::string nick);
	bool				_is_user_operator(std::string nick);
	void 				_return_topic(User &user);
	void				_msg_to_channel(User &user);

	const std::string				&getName() const;
	void							setName(const std::string &name);
	unsigned char 					getFlags() const;
	const std::vector<std::string> 	&getUsers() const;

	void 							setOperators(std::string &nick_user);
	void 							delOperators(std::string &nick_user);
	const std::vector<std::string> 	&getOperators() const;
	void 							setFlags(unsigned char flag);
};
