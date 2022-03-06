#pragma once

#include <string.h>
#include <iostream>
#include <netdb.h>

class Server {
private:
	fd_set			master, read_fds;	// главный сет дескрипторов и временный сет дескрипторов
	int				fdmax, listener; 				// fdmax - макс число дескрипторов
	int				newfd; // дескриптор для новых соединений
	struct addrinfo	hints, *res;
	socklen_t		addrlen;
	std::string		port;
	int				pass;
	std::string		host_ip;
public:
	Server(std::string port, int pass, std::string host_ip);

	void	init_server();
	void	start();
};