#pragma once

#include "../../inc/irc.hpp"
#include "../user/User.hpp"
<<<<<<< HEAD
// #include "irc.hpp"
=======
//#include "User.hpp"
//#include "irc.hpp"
>>>>>>> 9c5a24a32f4b79018ca7185e49537e4f05ef9966

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
	std::map<int, User *>			map_Users;
public:
	Server(std::string port, std::string pass, std::string host_ip);
	~Server();

	void	init_server();
	void	working_with_client(int fd);
	void	start();
	void	clear_disconnected();
private:
	void	print_ip();
};