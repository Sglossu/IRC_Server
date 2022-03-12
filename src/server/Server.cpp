#include "Server.hpp"

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server(std::string port, std::string pass, std::string host_ip) : port(port), pass(pass), host_ip(host_ip) {
	memset(act_set, 0, sizeof (act_set));
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
		critErr("selectserver");
	for (struct addrinfo *p = serv_addr_info; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, SOCK_STREAM, 0);
		if (listener < 0)
			continue;
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			critErr("selectserver: failed to bind with port =" + port);
		}
		break;
	}
	print_ip();
//	freeaddrinfo(serv_addr_info);
}

Server::~Server() {
	freeaddrinfo(serv_addr_info); // todo воткнуть куда-то
}

void working_with_client(int fd, int num_set, struct pollfd *act_set)
{
	int		nbytes;
	char	buf[256];

	if ((nbytes = recv(fd, buf, sizeof buf, 0)) <= 0)
	{
		// получена ошибка или соединение закрыто клиентом
		if (nbytes == 0)
			// соединение закрыто
			std::cerr << "selectserver: socket %d hung upn" << fd << std::endl;
		else{
			std::cout << fd << std::endl;
			throw "recv";
		}
		close(fd);
	}
	else
	{
		// у нас есть какие-то данные от клиента
		for(int j = 1; j < num_set; j++)
		{
			// отсылаем данные всем!
			if (act_set[j].fd != fd)
				if (send(act_set[j].fd, buf, nbytes, 0) == -1)
					throw "send";
		}
	}
}

void Server::start() {
	if (listen(listener, 10) < 0)
		throw "listen";

	act_set[0].fd = listener;
	act_set[0].events = POLLIN;
	act_set[0].revents = 0;
	num_set = 1;

	struct sockaddr_storage remoteaddr;
	socklen_t 				size_client = sizeof (remoteaddr);
//	char remoteIP[INET6_ADDRSTRLEN];

	while (true)
	{
		int ret = poll(act_set, num_set, -1);
		if (ret < 0)
			throw "server: poll failure";
		if (ret == 0)
		{
			std::cout << "Timeout" << std::endl;
			continue;
		}
		if (ret > 0)
		{
			for (int i = 0; i < num_set; i++)
			{
				if (act_set[i].revents & POLLIN)
				{
					std::cout << "POLLINT at fd " << act_set[i].fd << std::endl;
					act_set[i].revents &= ~POLLIN;
					if (i == 0)
					{
						// обработка нового соединения
//						inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);

						new_sock_fd = accept(act_set[i].fd, (struct sockaddr*)&remoteaddr, &size_client);
						std::cout << "New client on port " << port << std::endl;
						if (num_set < 100)
						{
							act_set[num_set].fd = new_sock_fd;
							act_set[num_set].events = POLLIN;
							act_set[num_set].revents = 0;
							num_set++;
						}
						else
							throw "no more sockets for clients";
					}
					else
					{
						// пришли данные, работаем с ними в существующем соединении
						working_with_client(act_set[i].fd, num_set, act_set);
					}
				}
			}
		}

	}
}