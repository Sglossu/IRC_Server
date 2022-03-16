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
//	print_ip();
//	freeaddrinfo(serv_addr_info);
}

Server::~Server() {
	freeaddrinfo(serv_addr_info); // todo воткнуть куда-то
}

void Server::working_with_client(int fd)
{
	int		nbytes;
	char	buf[512];

	if (!map_Users.count(fd)) {
		std::cout << "Unprocees errors: not found user with this fd-key" << std::endl;
		return ;
	}
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
		map_Users.erase(fd);
	}
	else
	{
		// у нас есть какие-то данные от клиента
		map_Users[fd].processIncommingMessage(buf);

		for(size_t j = 1; j < act_set.size(); j++)
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

	pollfd	new_Pollfd = {listener, POLLIN, 0};
	act_set.push_back(new_Pollfd);
	// std::vector<struct pollfd>::iterator	iter = act_set.begin();

	struct sockaddr_storage remoteaddr;
	socklen_t 				size_client = sizeof (remoteaddr);

	while (true)
	{
		struct pollfd * act_set_pointer = &act_set[0];
		int ret = poll(act_set_pointer, act_set.size(), -1);
		if (ret < 0)
			throw "server: poll failure";
		if (ret == 0)
		{
			std::cout << "Timeout" << std::endl;
			continue;
		}
//______________________ПЕРВЫЙ ВАРИАНТ. ЗДЕСЬ НА 125 СТРОКЕ ОШИБКА - ПОЧЕМУ-ТО it_pollfd ВО 2Й ИТЕРАЦИИ ХРАНИТ ФИГНЮ_________

//		if (ret > 0)
//		{
//			pollfd	i;
//			for (std::vector<pollfd>::iterator it_pollfd = act_set.begin(); it_pollfd != act_set.end(); it_pollfd++)
//			{
//				i = *it_pollfd;// todo why вторая итерация даёт не те значения??
//				if (i.revents & POLLIN)
//				{
//					std::cout << "POLLINT at fd " << i.fd << std::endl;
//					i.revents &= ~POLLIN;
//					if (i.fd == act_set[0].fd) // проверка что это listening
//					{
//						// обработка нового соединения
//						new_sock_fd = accept(act_set[0].fd, (struct sockaddr*)&remoteaddr, &size_client);
//						map_Users.insert(std::pair<int, User>(new_sock_fd, User(new_sock_fd)));
//						std::cout << "New client on port " << port << std::endl;
//						new_Pollfd = {new_sock_fd, POLLIN, 0};
//						act_set.push_back(new_Pollfd);
//					}
//					else
//					{
//						// пришли данные, работаем с ними в существующем соединении
//						working_with_client(i.fd);
//					}
//				}
//			}
//		}

//_________________________ВТОРОЙ ВАРИАНТ: ЗДЕСЬ ДЛЯ НОВЫХ ПОДКЛЮЧЕНИЙ НЕ МЕНЯЕТСЯ REVENTS______________
		if (ret > 0)
		{
			for (size_t i = 0; i < act_set.size(); i++)
			{
				if (act_set[i].revents & POLLIN)
				{
					std::cout << "POLLINT at fd " << act_set[i].fd << std::endl;
					act_set[i].revents &= ~POLLIN;
					if (act_set[i].fd == act_set[0].fd) // проверка что это listening
					{
						// обработка нового соединения
						new_sock_fd = accept(act_set[0].fd, (struct sockaddr*)&remoteaddr, &size_client);
						// а он может кривой инт вернуть? не помню
						map_Users[new_sock_fd] = User(new_sock_fd);
						std::cout << "New client on port " << port << std::endl;
						new_Pollfd = {new_sock_fd, POLLIN, 0};
						act_set.push_back(new_Pollfd);
					}
					else
					{
						// пришли данные, работаем с ними в существующем соединении
						working_with_client(act_set[i].fd);
					}
				}
			}
		}

	}
}