#include "Handler.hpp"

Handler::Handler(Server *server) {
	_pserver = server;
	_pmap_users = &server->getMapUsers();
}

Handler::~Handler() {}

Server *Handler::getPserver() const {
	return _pserver;
}

std::map<int, User *> *Handler::getPmapUsers() const {
	return _pmap_users;
}
