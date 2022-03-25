#pragma once
#include "../../inc/irc.hpp"
#include "../server/Server.hpp"
#include "../user/User.hpp"
#include "../message/Message.hpp"
#include "sha256/Sha256.hpp"
#include "yaml_parser/YamlParser.hpp"

typedef  void (Handler::*cmd_func) ( Message &, User &);

class Handler {

private:
	Server							&_server;
	std::map<int, std::string>		_bufs; // буфер - содержит сообщения по кусочкам, если они таковыми пришли
	std::map<std::string, cmd_func>	_commands;
	YamlParser						_config;


	// std::map<int, User *>			*_pmap_users;

	bool		_is_valid_nick(std::string nick);
	bool		_is_valid_groupname(std::string	name);


	void				_registration_commands();
	void				_test1(Message &msg, User &user);

	void		_cmd_pass(Message &msg, User &user);
	void 		_cmd_nick(Message &msg, User &user);
	void 		_cmd_user(Message &msg, User &user);
	void		_cmd_oper(Message &msg, User &user);
	void		_cmd_quit(Message &msg, User &user);
	
	void		_cmd_privmsg(Message &msg, User &user);


public:
	Handler(Server &server);

	~Handler();

	void				process_incomming_message(int fd, std::string buf);

	const YamlParser	&getConfig() const;
	bool				check_registration(Message *msg, User &user);
	void				clear_buf(int fd);
	void				_error_msg(Message *msg, User *user, int er);
	void 				_cmd_responses(Message *msg, User *user, int er);
	
};

