#pragma once

#include "irc.hpp"

#include <string>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>


#define TIMEOUT 1001

class Server {
private:
	struct pollfd	act_set[100];
	int				listener; 				// fdmax  - макс число дескрипторов
	struct addrinfo	hints, *serv_addr_info;
	std::string		port;
	std::string		pass;
	std::string		host_ip;
	int				num_set; // количество fd
	int				new_sock_fd;
public:
	Server(std::string port, std::string pass, std::string host_ip);
	~Server();

	void	init_server();
	void	start();
private:
	void	print_ip();
};