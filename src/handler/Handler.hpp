#pragma once
#include "../../inc/irc.hpp"
#include "../server/Server.hpp"
#include "../user/User.hpp"
#include "../message/Message.hpp"

typedef  void (Handler::*cmd_func) ( Message &, User &);

class Handler {

private:
	Server							&_server;
	std::map<int, std::string>		_bufs;
	std::map<std::string, cmd_func>	_commands;
	// std::map<int, User *>			*_pmap_users;

	bool	_is_valid_nick(std::string nick);
	bool	_is_valid_groupname(std::string	name);


	void	_registration_commands();
	void	_test1(Message &msg, User &user);

	void	_cmd_pass(Message &msg, User &user);
	void	_cmd_nick(Message &msg, User &user);
	void	_cmd_user(Message &msg, User &user);



public:
	Handler(Server &server);
	~Handler();

	void	process_incomming_message(int fd, std::string buf);
	void	clear_buf(int fd);

	
};

