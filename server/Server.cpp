#include "Server.hpp"

Server::Server(std::string port, int pass, std::string host_ip) : port(port), pass(pass), host_ip(host_ip) {
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
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
	if(getaddrinfo(host_ip.c_str(), port.c_str(), &hints, &serv_addr_info))
	{
		std::cerr << "selectserver" << std::endl;
		exit(1);
	}
	listener = socket(PF_INET, SOCK_STREAM, 0);
	print_ip();

}

Server::~Server() {
	freeaddrinfo(serv_addr_info); // todo воткнуть куда-то
}

void Server::start() {

}