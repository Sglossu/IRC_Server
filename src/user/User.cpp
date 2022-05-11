#include "User.hpp"

User::User(int fd_sock) : _fd_sock(fd_sock) {
	_flags = 0;
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

void	User::join_channel(std::string &channel_name) {
	_chanels.push_back(channel_name);
	std::cout << "User <" << _nick << "> join channel " << channel_name << std::endl;
}


const unsigned char   User::get_flags() const {return _flags;}

void User::del_flag(unsigned char flag) {
	_flags &= ~flag;
}

const int User::getFdSock() const {
	return _fd_sock;
}

void User::setNick(const std::string &nick) {
	_nick = nick;
}

const std::string &User::getNick() const {
	return _nick;
}

const std::string &User::getUsername() const {
	return _username;
}

void User::setUsername(const std::string &username) {
	_username = username;
}

const std::string &User::getHostname() const {
	return _hostname;
}

void User::setHostname(const std::string &hostname) {
	_hostname = hostname;
}

const std::string &User::getServername() const {
	return _servername;
}

void User::setServername(const std::string &servername) {
	_servername = servername;
}

const std::string &User::getRealname() const {
	return _realname;
}

void User::setRealname(const std::string &realname) {
	_realname = realname;
}

const std::string &User::getRplAway() const {
	return _rplAway;
}

void User::setRplAway(const std::string &rplAway) {
	_rplAway = rplAway;
}

const std::string &User::getPass() const {
	return _pass;
}

void User::setPass(const std::string &pass) {
	_pass = pass;
}

const std::vector<std::string> &User::getChanels() const {
	return _chanels;
}

// добавляет сообщение для отправки пользователю. Максимально за раз отправляется до 512 байт
void	User::addMsgToSend(const std::string &msg) {
	if (_to_send.size() and (_to_send[_to_send.size() - 1].size() + msg.size() < 512))
		_to_send[_to_send.size() - 1] += msg;
	else
		_to_send.push_back(msg);
}

// удаляет уже отправленное сообщение из очереди
void	User::clearFirstElementToSend() {
	if (_to_send.size())
		_to_send.erase(_to_send.begin());
}

bool	User::haveMsgToSend() const{
	return bool(_to_send.size());
}

const std::string	&User::getOneElementToSend() const {
	return _to_send[0];
}
