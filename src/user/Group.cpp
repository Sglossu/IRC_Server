#include "Group.hpp"

Group::Group(std::string name, std::string &owner) {
	_users.push_back(owner);
}

Group::~Group() {
	//todo: удаляем всех пользователей с группы через KICK?
}


void Group::delete_user(std::string &username) {
	std::vector<std::string>::iterator it;

	it = std::find(begin(_users), end(_users), username);
	if (it != _users.end()) {
		// todo: send user left the group
		_users.erase(it);
	}

}

const std::vector<std::string> &Group::get_users() const {return _users; }

