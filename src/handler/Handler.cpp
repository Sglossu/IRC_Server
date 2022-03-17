#include "Handler.hpp"

Handler::Handler(Server *server) {
	_pserver = server;
	_pmap_users = &server->getMapUsers();
}

Handler::~Handler() {}

void Handler::start_game() {
	_pserver->start();

	if (listen(_pserver->listener, 10) < 0)
		throw "listen";

	pollfd	new_Pollfd = {_pserver->listener, POLLIN, 0};
	_pserver->act_set.push_back(new_Pollfd);

	struct sockaddr_storage remoteaddr;
	socklen_t 				size_client = sizeof (remoteaddr);

	while(true)
	{
		struct pollfd * act_set_pointer = &_pserver->act_set[0]; // указатель на первый элемент вектора act_set
		int ret = poll(act_set_pointer, _pserver->act_set.size(), -1);
		if (ret < 0)
			throw "server: poll failure";
		else if (ret == 0)
		{
			std::cout << "Timeout" << std::endl;
			continue;
		}

		else if (ret > 0)
		{
			for (size_t i = 0; i < _pserver->getActSet().size(); i++)
			{
				if (_pserver->getActSet()[i].revents & POLLIN)
				{
					_pserver->new_connection(i, remoteaddr, size_client);
				}
			}
		}
		_pserver->clear_disconnected();
	}
}

Server *Handler::getPserver() const {
	return _pserver;
}

std::map<int, User *> *Handler::getPmapUsers() const {
	return _pmap_users;
}
