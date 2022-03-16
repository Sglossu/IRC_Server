#pragma once

// #include "../../inc/irc.hpp"
#include "../user/User.hpp"
#include "irc.hpp"

#include <string>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>


#define TIMEOUT 1000

class Server {
private:
	std::vector<struct pollfd>	act_set;
	int							listener; 				// fdmax  - макс число дескрипторов
	struct addrinfo				hints, *serv_addr_info;
	std::string					port;
	std::string					pass;
	std::string					host_ip;
//	int							num_set; // количество fd
	int							new_sock_fd;
	std::map<int, User>			map_Users;
public:
	Server(std::string port, std::string pass, std::string host_ip);
	~Server();

	void	init_server();
	void	working_with_client(int fd);
	void	start();
private:
	void	print_ip();
};