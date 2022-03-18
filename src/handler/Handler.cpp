#include "Handler.hpp"

Handler::Handler(Server &server): _server(server) {
	std::cout << "handler constructor. server port: " << server.port << std::endl;
}

Handler::~Handler() {}


void	Handler::process_incomming_message(int fd, std::string buf) {
	size_t      pos;
	std::string msg_line;

	if (_bufs.count(fd))
		_bufs[fd] += buf;
	else
		_bufs[fd] = buf;
	std::cout << "User <" << fd << ", " << _server.map_users[fd]
			  << "> incoming msg(" << _bufs[fd].size() <<"): "  << _bufs[fd] << std::endl;
	while (!_bufs[fd].empty()) {
		pos = _bufs[fd].find(CR_LF);
		if (pos == _bufs[fd].npos)
			break ;
		msg_line = _bufs[fd].substr(0, pos);
		_bufs[fd] = _bufs[fd].substr(pos + 2); // раскоменить когда считывается /r
//		_msg_buf = _msg_buf.substr(pos + 1); // закоментить, если верхняя строка раскоменчена

		parce_comand(fd, msg_line); // тут можете хоть класс сделать
	}
}

void	Handler::parce_comand(int fd, std::string cmnd) {
	std::cout << "User <fd " << fd << _server.map_users[fd]->get_name() << "> parce cmnd: "  << cmnd << std::endl;
	// todoshen'ka
}


void	Handler::clear_buf(int fd) {
	if (_bufs.count(fd))
		;// erase fucking buff ^^;
}
// void Handler::start_game() {
// 	_pserver->start();

// 	if (listen(_pserver->listener, 10) < 0)
// 		throw "listen";

// 	pollfd	new_Pollfd = {_pserver->listener, POLLIN, 0};
// 	_pserver->act_set.push_back(new_Pollfd);

// 	struct sockaddr_storage remoteaddr;
// 	socklen_t 				size_client = sizeof (remoteaddr);

// 	while(true)
// 	{
// 		struct pollfd * act_set_pointer = &_pserver->act_set[0]; // указатель на первый элемент вектора act_set
// 		int ret = poll(act_set_pointer, _pserver->act_set.size(), -1);
// 		if (ret < 0)
// 			throw "server: poll failure";
// 		else if (ret == 0)
// 		{
// 			std::cout << "Timeout" << std::endl;
// 			continue;
// 		}

// 		else if (ret > 0)
// 		{
// 			for (size_t i = 0; i < _pserver->getActSet().size(); i++)
// 			{
// 				if (_pserver->getActSet()[i].revents & POLLIN)
// 				{
// 					_pserver->new_connection(i, remoteaddr, size_client);
// 				}
// 			}
// 		}
// 		_pserver->clear_disconnected();
// 	}
// }