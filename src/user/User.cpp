#include "User.hpp"

User::User(int fd_sock) : _fd_sock(fd_sock) {
	_flags = 0;
	_pass = false;

}


User::~User() {
	std::cout << "delete constructor called, named = " << this->_username << std::endl;
	// todo: чистка пользователя со всех групп
	// todo: отправка сообщения об отключении
}


void	User::set_flag(unsigned char flag)
{
	_flags |= flag;
}

void	User::join_group(std::string &group_name) {
	// временный джоин для реализации работы логики групп

	// todo проверка что имя группы корректно
	// todo проверка что пользователь есть. 
	// что группа есть.
	// что его там нет
	// что он может туда присоединиться

	std::cout << "join group " << group_name << std::endl;
	
}


const unsigned char   User::get_flags() const {return _flags;}

const std::string     &User::get_name() const { return _username; }

const int User::getFdSock() const {
	return _fd_sock;
}

void User::setNick(const std::string &nick) {
	_nick = nick;
}

const std::string &User::getNick() const {
	return _nick;
}
