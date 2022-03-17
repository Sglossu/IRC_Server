#pragma once

#include "../../inc/irc.hpp"
#include "../user/User.hpp"

//#include "User.hpp"
//#include "irc.hpp"


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
	// std::set<int>				fd_to_del;
	int							listener; 				// fdmax  - макс число дескрипторов
	struct addrinfo				hints, *serv_addr_info;
	std::string					port;
	std::string					pass;
	std::string					host_ip;
//	int							num_set; // количество fd
	int							new_sock_fd;
	std::map<int, User *>			map_users;
public:
	Server(std::string port, std::string pass, std::string host_ip);
	~Server();

	void	init_server();
	void	working_with_client(int fd);
	void	start();
	void	clear_disconnected();
	void	new_connection(int i, struct sockaddr_storage remoteaddr, socklen_t size_client);

	std::map<int, User *> &getMapUsers();

	std::vector<struct pollfd> &getActSet();

private:
	void	print_ip();
};


