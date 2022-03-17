#include "User.hpp"

User::User(int fd_sock) : _fd_sock(fd_sock) {
	_flags = 0;
}


User::~User() {
	// чистка пользователя со всех групп
	// отправка сообщения об отключении
}

void	User::processIncommingMessage(std::string buf) {
	size_t      pos;
	std::string msg_line;

	_msg_buf += buf;
	const char *s = buf.c_str();
	printf(" |\n");
	for (size_t i = 0; i < buf.size(); i++) {
		printf(" %d", s[i]);
		if (s[i] == '\n')
			printf("\n");
	}
	printf(" |\n");
	std::cout << "User <" << _fd_sock << ", " << _username
			  << "> incooming msg(" << _msg_buf.size() <<"): "  << _msg_buf << std::endl;
	while (1) {
		pos = _msg_buf.find(CR_LF);
		std::cout << "pos = " << pos << std::endl;
		if (pos == _msg_buf.npos)
			break ;
		msg_line = _msg_buf.substr(0, pos);
		_msg_buf = _msg_buf.substr(pos + 2);
		parceComand(msg_line);
	}
}

void	User::parceComand(std::string cmnd) {
	std::cout << "User <%d, " << _fd_sock << _username << "> parce cmnd: "  << cmnd << std::endl;
	// todoshen'ka
}

void	User::set_flag(unsigned char flag)
{
	_flags |= flag;
}

const unsigned char   User::get_flags() const {return _flags;}

const std::string     &User::get_name() const { return _username; }
