#include "Server.hpp"
#include "../handler/Handler.hpp"

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server(std::string port, std::string pass, std::string host_ip) : port(port), pass(pass), host_ip(host_ip), handler(new Handler(*this)) {
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // неважно v4 или v6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // назначение сокету адрес хоста
	// handler = new Handler(this);
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
		this->ipstr.push_back(ipstr);
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
	handler->setHost(ipstr[1]);
}

Server::~Server() {
	freeaddrinfo(serv_addr_info); // todo воткнуть куда-то
}

void Server::write_to_client(int fd, const std::string &msg) {
	int nbytes = msg.size();

	if (send(fd, msg.c_str(), nbytes, 0) == -1)
				throw "send";
}

void Server::write_to_client(std::string nick, const std::string &msg) { // todo before using need fix segfault with unknown nicks
	int nbytes = msg.size();
//	std::cout << "send by nick " << nick << std::endl;
	User *recv =  mapnick_users[nick];
	if (recv and recv->getFdSock() != -1) {
		int fd = recv->getFdSock();
		if (send(fd, msg.c_str(), nbytes, 0) == -1)
			throw "send";
	}
}

void Server::working_with_client(int fd)
{
	int		nbytes;
	char	buf[513];

	if (!mapfd_users.count(fd)) {
		std::cout << "Unprocees errors: not found user with this fd-key" << std::endl;
		return ;
	}
	if ((nbytes = recv(fd, buf, 512, 0)) <= 0)
	{
		// получена ошибка или соединение закрыто клиентом
		if (nbytes == 0)
			// соединение закрыто
			std::cerr << "selectserver: socket"  << fd << " hung up"<<  std::endl;
		else{
			std::cout << fd << std::endl;
			throw "recv";
		}
		mapfd_users[fd]->set_flag(DISCONNECTED);
	}
	else
	{
		// у нас есть какие-то данные от клиента
		buf[nbytes] = 0;
		if (!(mapfd_users[fd]->get_flags() & DISCONNECTED))
			handler->process_incomming_message(fd, buf);

//		 for(size_t j = 1; j < act_set.size(); j++)
//		 {
//		 	// отсылаем данные всем!
//		 	if (act_set[j].fd != fd)
//		 		if (send(act_set[j].fd, buf, nbytes, 0) == -1)
//		 			throw "send";
//		 }
	}
}


void Server::start() {
	if (listen(listener, 10) < 0)
		throw "listen";

	pollfd	new_Pollfd = {listener, POLLIN, 0};
	act_set.push_back(new_Pollfd);

	struct sockaddr_in remoteaddr;
	socklen_t 				size_client = sizeof (remoteaddr);

	while (true)
	{
		struct pollfd * act_set_pointer = &act_set[0]; // указатель на первый элемент вектора act_set
		int ret = poll(act_set_pointer, act_set.size(), -1);
		if (ret < 0)
			throw "server: poll failure";
		else if (ret == 0)
		{
			std::cout << "Timeout" << std::endl;
			continue;
		}

		else if (ret > 0)
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
                        std::cout << "user pi " << remoteaddr.sin_addr.s_addr << std::endl;
						// а он может кривой инт вернуть? не помню
						mapfd_users[new_sock_fd] = new User(new_sock_fd);
						std::cout << "New client on port " << port << std::endl;
						struct pollfd new_Pollfd = {new_sock_fd, POLLIN, 0};
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
		// Этнические чистки
		clear_disconnected();


	}
}


// Удаляет сломанные фдшники и отключенных пользователей
void	Server::clear_disconnected() {
	std::vector<struct pollfd>::iterator it;

	it = act_set.begin();
	for (size_t i = 1; i < act_set.size(); i++)
		if (mapfd_users[act_set[i].fd]->get_flags() & DISCONNECTED) {
			mapfd_users.erase(act_set[i].fd);
			handler->clear_buf(act_set[i].fd);
			close(act_set[i].fd);
			act_set.erase(it + i);
			--i;
		}
	// printf("len pollfd after cleaning %zu\n len users %zu\n", act_set.size(), mapfd_users.size());
}

bool	Server::_is_user_on_channel(std::string channel, std::string nick) {
	if (map_channels[channel]->_is_user_on_channel(nick))
		return true;
	return false;
}

bool	Server::is_nick_exist(std::string &nick) {
	return true;
}

const std::string &Server::getPass() const {
	return pass;
}

const std::vector<std::string> &Server::getIpstr() const {
	return ipstr;
}




