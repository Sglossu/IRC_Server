#include "Channel.hpp"

Сhannel::Сhannel(std::string name, std::string &owner) {
	_users.push_back(owner);
}

Сhannel::~Сhannel() {
	//todo: удаляем всех пользователей с группы через KICK?
}


void Сhannel::delete_user(std::string &username) {
	std::vector<std::string>::iterator it;

	it = std::find(begin(_users), end(_users), username);
	if (it != _users.end()) {
		// todo: send user left the channel
		_users.erase(it);
	}

}

const std::vector<std::string> &Сhannel::get_users() const {return _users; }

