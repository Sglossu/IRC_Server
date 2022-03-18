#pragma once
#include "../../inc/irc.hpp"
#include "../server/Server.hpp"
#include "../user/User.hpp"


class Handler {

private:
	Server						&_server;
	std::map<int, std::string>	_bufs;
	// std::map<int, User *>			*_pmap_users;
public:
	Handler(Server &server);
	~Handler();

	void	process_incomming_message(int fd, std::string buf);
	void	parce_comand(int fd, std::string cmnd);

	void	clear_buf(int fd);
	// Server *getPserver() const;
	// std::map<int, User *> *getPmapUsers() const;
	// void	start_game();
};

