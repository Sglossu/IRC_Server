#pragma once

#include <string.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>

#define TIMEOUT 1000

class Server {
private:
//	fd_set			master, read_fds;	// главный сет дескрипторов и временный сет дескрипторов
//	int				fdmax;
	struct pollfd	fds[2];
	int				listener; 				// fdmax - макс число дескрипторов
	int				newfd; // дескриптор для новых соединений
	struct addrinfo	hints, *serv_addr_info;
	socklen_t		addrlen;
	std::string		port;
	int				pass;
	std::string		host_ip;
	int				numbers_fd;
public:
	Server(std::string port, int pass, std::string host_ip);
	~Server();

	void	init_server();
	void	start();
private:
	void	print_ip();
};