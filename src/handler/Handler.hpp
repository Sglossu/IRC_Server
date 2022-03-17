#pragma once
#include "../../inc/irc.hpp"
#include "../server/ServerException.hpp"
#include "../server/Server.hpp"
#include "../user/User.hpp"

class Handler {
private:
	Server							*_pserver;
	std::map<int, User *>			*_pmap_users;
public:
	Handler(Server *server);
	~Handler();

	Server *getPserver() const;
	std::map<int, User *> *getPmapUsers() const;
	void	start_game();
};

