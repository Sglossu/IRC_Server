#pragma once

#include "../../inc/irc.hpp"
#include "../user/User.hpp"
#include "../user/Channel.hpp"
// #include "../handler/Handler.hpp"

//#include "User.hpp"
//#include "irc.hpp"


#include <string>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>

#define TIMEOUT 1000

class Handler;

class Server {
	friend class	Handler;

private:
	const std::string					port;
	const std::string					pass;
	const std::string					host_ip;
	Handler								*handler;
	std::vector<struct pollfd>			act_set;
	int									listener;
	struct addrinfo						hints, *serv_addr_info;
	int									new_sock_fd;
	std::map<int, User *>				mapfd_users;
	std::map<std::string, User *>		mapnick_users;
	std::map<std::string, Channel *>	map_channels;
	std::vector<std::string>			ipstr;

	void							working_with_client(int fd);
	void							clear_disconnected();
	void							print_ip();

public:
	Server(std::string port, std::string pass, std::string host_ip);
	~Server();

	void							init_server();
	void							start();

	const std::string 				&getPass() const;
	void							write_to_client(int fd, const std::string &msg);
	void							write_to_client(std::string nick, const std::string &msg);
	bool							is_nick_exist(std::string &nick);
	bool							_is_user_on_channel(std::string channel, std::string nick);
	const std::string				&getLastIpstr() const;


	const std::vector<std::string> &getIpstr() const;
};


