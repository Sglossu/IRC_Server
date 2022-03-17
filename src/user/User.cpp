#include "User.hpp"

User::User(int fd_sock) : _fd_sock(fd_sock) {
	_flags = 0;
	_pass = false;

}


User::~User() {
	// чистка пользователя со всех групп
	// отправка сообщения об отключении
}

void	User::processIncommingMessage(std::string buf) {
	size_t      pos;
	std::string msg_line;

	_msg_buf += buf;
//	const char *s = buf.c_str();
//	printf(" |\n");
//	for (size_t i = 0; i < buf.size(); i++) {
//		printf(" %c", s[i]);
//		if (s[i] == '\n')
//			printf("\n");
//	}
//	printf(" |\n");
//	std::cout << "User <" << _fd_sock << ", " << _username
//			  << "> incoming msg(" << _msg_buf.size() <<"): "  << _msg_buf << std::endl;
	while (!_msg_buf.empty()) {
		pos = _msg_buf.find(CR_LF);
		std::cout << "pos = " << pos << std::endl;
		if (pos == _msg_buf.npos)
			break ;
		msg_line = _msg_buf.substr(0, pos);
		_msg_buf = _msg_buf.substr(pos + 2); // раскоменить когда считывается /r
//		_msg_buf = _msg_buf.substr(pos + 1); // закоментить, если верхняя строка раскоменчена
		parceComand(msg_line);
	}
}

void	User::parceComand(std::string cmnd) {
	std::cout << "User <fd " << _fd_sock << _username << "> parce cmnd: "  << cmnd << std::endl;
	// todoshen'ka
//	if (!_pass)

}

void	User::set_flag(unsigned char flag)
{
	_flags |= flag;
}

const unsigned char   User::get_flags() const {return _flags;}

const std::string     &User::get_name() const { return _username; }
