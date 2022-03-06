#include "Server.hpp"

Server::Server(std::string port, int pass, std::string host_ip) : port(port), pass(pass), host_ip(host_ip) {
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // неважно v4 или v6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // назначение сокету адрес хоста
}

void Server::init_server() {
	listener = socket(PF_INET, SOCK_STREAM, 0);
	getaddrinfo(host_ip.c_str(), port.c_str(), &hints, &res);
}

void Server::start() {

}