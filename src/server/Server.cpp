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
			ipver = (char *)"IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = (char *)"IPv6";
		}
		char buf_ipstr[INET6_ADDRSTRLEN];
		// преобразуем IP в строку и выводим его:
		inet_ntop(p->ai_family, addr, buf_ipstr, sizeof buf_ipstr);
		this->ipstr.push_back(buf_ipstr);
		printf(" %s: %s\n", ipver, buf_ipstr);

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
	handler->setHost(getLastIpstr());
}

Server::~Server() {
	freeaddrinfo(serv_addr_info); // todo воткнуть куда-то
}

void Server::write_to_client(int fd, const std::string &msg) {
	if (DEBUG)
		std::cout << "--> send by fd " << fd << ": " << msg;
	User *recv =  mapfd_users[fd];
	if (recv->getFdSock() != -1) 
		recv->addMsgToSend(msg);

	// int nbytes = msg.size();
	// try {
	// 	if (send(fd, msg.c_str(), nbytes, 0) == -1)
	// 			throw "fail send";
	// }
	// catch (const std::exception &e) {
    // 	std::cerr << "possible broken pipe" << e.what();
	// }
}

void Server::write_to_client(std::string nick, const std::string &msg) { // todo before using need fix segfault with unknown nicks
	// int nbytes = msg.size();
	std::cout << "--> send by nick " << nick << ": " << msg;
	if (!mapnick_users.count(nick))
		return ;
	User *recv =  mapnick_users[nick];
	if (recv->getFdSock() != -1) {
		recv->addMsgToSend(msg);
		// int fd = recv->getFdSock();

		// try {
		// 	if (send(fd, msg.c_str(), nbytes, 0) == -1)
		// 		throw "send";
		// }
		// catch (const std::exception &e) { // хорошо бы добавить отлов исключения broken pipe
    	// 	std::cerr << "possible broken pipe" << e.what();
		// }
	}
}

void Server::broadcast_message(std::string &msg) {
	for (size_t i = 1; i < act_set.size(); i++)
		write_to_client(act_set[i].fd, msg);
}

void Server::working_with_client(int fd)
{
	int		nbytes;
	char	buf[513];

	if (!mapfd_users.count(fd)) {
		std::cout << "Unprocees errors: not found user with this fd-key" << std::endl;
		return ;
	}
	if ((nbytes = recv(fd, buf, 512, 0)) <= 0) {
		// получена ошибка или соединение закрыто клиентом
		if (nbytes == 0) {
			// соединение закрыто
			mapfd_users[fd]->set_flag(DISCONNECTED);
			std::cerr << "selectserver: socket" << fd << " hung up" << std::endl;
		}
		else{
			std::cout << fd << std::endl;
			throw "recv";
		}
		mapfd_users[fd]->set_flag(DISCONNECTED);
	}
	else {
		// у нас есть какие-то данные от клиента
		buf[nbytes] = 0;
		if (!(mapfd_users[fd]->get_flags() & DISCONNECTED))
			handler->process_incomming_message(fd, buf);
	}
}

void	Server::send_msg_to_client(int fd) {
	std::string msg = mapfd_users[fd]->getOneElementToSend();
	int nbytes = msg.size();
	try {
		if (send(fd, msg.c_str(), nbytes, 0) == -1)
			throw "fail send";
		mapfd_users[fd]->clearFirstElementToSend();
	}
	catch (const std::exception &e) {
    	std::cerr << "possible broken pipe" << e.what();
	}

}

void Server::start() {
	if (listen(listener, 10) < 0)
		throw "listen";

	fcntl(listener, F_SETFL, O_NONBLOCK);
	pollfd	new_Pollfd = {listener, POLLIN, 0};
	act_set.push_back(new_Pollfd);

	struct sockaddr_in remoteaddr;
	socklen_t	size_client = sizeof (remoteaddr);

	while (true)
	{
		struct pollfd * act_set_pointer = &act_set[0]; // указатель на первый элемент вектора act_set
		int ret = poll(act_set_pointer, act_set.size(), -1);
		if (ret < 0)
			throw "server: poll failure";
		else if (ret == 0) {
			std::cout << "Timeout" << std::endl;
		}

		else if (ret > 0)
		{
			for (size_t i = 0; i < act_set.size(); i++)
			{
				if (act_set[i].revents & POLLOUT) {
					send_msg_to_client(act_set[i].fd);
					act_set[i].events = POLLIN;
				}
				if (act_set[i].revents & POLLIN)
				{
					std::cout << "POLLINT at fd " << act_set[i].fd << std::endl;
					act_set[i].revents &= ~POLLIN;
					if (act_set[i].fd == act_set[0].fd) // проверка что это listening
					{
						// обработка нового соединения
						new_sock_fd = accept(act_set[0].fd, (struct sockaddr*)&remoteaddr, &size_client);
						fcntl(new_sock_fd, F_SETFL, O_NONBLOCK);
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
				// ошибка на соединении
				else if (act_set[i].revents & POLLNVAL or act_set[i].revents & POLLHUP or act_set[i].revents & POLLERR) {
					if (act_set[i].fd != act_set[0].fd)
						mapfd_users[act_set[i].fd]->set_flag(DISCONNECTED);
				}
			}
		}
		// Этнические чистки
		clear_disconnected();
		add_clients_to_send();
	}
}

// отмечаем клиентов для отправки, если для них есть сообщения
void	Server::add_clients_to_send() {
	for (size_t i = 1; i < act_set.size(); i++)
		if (mapfd_users[act_set[i].fd]->haveMsgToSend())
			act_set[i].events |= POLLOUT;
}

// Удаляет сломанные фдшники и отключенных пользователей
void	Server::clear_disconnected() {
	std::vector<struct pollfd>::iterator it;

	it = act_set.begin();
	for (size_t i = 1; i < act_set.size(); i++)
		if (mapfd_users[act_set[i].fd]->get_flags() & DISCONNECTED) {
			User *user = mapfd_users[act_set[i].fd];
			
			// если есть неотправленные сообщения для юзера - то не отключаем сразу
			// а даем несколько попыток для отправки сообщения
			if (user->haveMsgToSend() and user->getAttemp() < ATTEMP_TO_DISCONNECT) {
				user->setAttemp(user->getAttemp() + 1);
				continue;
			}
			// удаление из каналов
			std::vector<std::string>	channels_user = user->getChanels();
			std::string 				nick_user = user->getNick();
			for (size_t j = 0; j < channels_user.size(); j++) 
				if (map_channels.count(channels_user[j])) {
					map_channels[channels_user[j]]->_delete_user(nick_user);
					handler->_write_to_channel(channels_user[j], *user, "PART " + channels_user[j]);
				}
			delete user;
			mapfd_users.erase(act_set[i].fd);
			mapnick_users.erase(nick_user);
			handler->clear_buf(act_set[i].fd);
			close(act_set[i].fd);
			act_set.erase(it + i);
			--i;
		}
	std::map<std::string, Channel *>:: iterator it_map = map_channels.begin();
	while (it_map != map_channels.end()) {
		if (!it_map->second->getOperators().size()) {
			if (DEBUG)
				std::cout << RED"DELETE channel from clear_disconnected " << it_map->second->getName() << RESET << std::endl;
			delete it_map->second;
			it_map = map_channels.erase(it_map);
		}
		else
			it_map++;

	}


	// printf("len pollfd after cleaning %zu\n len users %zu\n", act_set.size(), mapfd_users.size());
}

bool	Server::_is_user_on_channel(std::string channel, std::string nick) {
	if (map_channels[channel]->_is_user_on_channel(nick))
		return true;
	return false;
}

// безопасно возвращает последний из полученых адресов сервера
const std::string	&Server::getLastIpstr() const {
	if (this->ipstr.size())
		return this->ipstr[ipstr.size() - 1];
	throw "Ip address not set";
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




