#include "Server.hpp"

Server::Server(std::string port, int pass, std::string host_ip) : port(port), pass(pass), host_ip(host_ip) {
	memset(fds, 0, sizeof (fds));
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // неважно v4 или v6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // назначение сокету адрес хоста
}

void Server::print_ip() {
	for(struct addrinfo *p = serv_addr_info; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		// получаем указатель на адрес, по разному в разных протоколах
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		char ipstr[INET6_ADDRSTRLEN];
		// преобразуем IP в строку и выводим его:
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf(" %s: %s\n", ipver, ipstr);
	}
}

void Server::init_server() {
	int yes = 1;
	if(getaddrinfo(host_ip.c_str(), port.c_str(), &hints, &serv_addr_info))
	{
		std::cerr << "selectserver" << std::endl;
		exit(1);
	}
	for (struct addrinfo *p = serv_addr_info; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, SOCK_STREAM, 0);
		if (listener < 0)
			continue;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			std::cerr << "selectserver: failed to bindn" << std::endl;
			exit(2);
		}
		break;
	}
	print_ip();
	freeaddrinfo(serv_addr_info);
}

Server::~Server() {
	freeaddrinfo(serv_addr_info); // todo воткнуть куда-то
}

void Server::start() {
	if (listen(listener, 10) < 0)
		throw "listen";

	fds[0].fd = 0;
	fds[0].events = POLLIN;
	fds[1].fd = 1;
	fds[1].events = POLLOUT;

	while (true)
	{
		numbers_fd = poll(fds, 2, TIMEOUT);
		if (numbers_fd < 0)
			throw "poll";
		if (numbers_fd == 0)
		{
			std::cout << "TIMEOUT" << std::endl;
			continue;
		}
		if (fds[0].revents & POLLIN)
		{

			fds[0].revents = 0;
		}
		if (fds[1].revents & POLLOUT)
			fds[1].revents = 1;
	}
}