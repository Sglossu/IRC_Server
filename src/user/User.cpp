#include "User.hpp"

User::User(int fd_sock) : _fd_sock(fd_sock) {}

User::User(const User &other) {
    _fd_sock = other._fd_sock;
    _username = other._username;
    _realname = other._realname;
    _nick = other._nick;
    _message = other._message;

}

User& User::operator=(const User& other) {
    if (this == &other)
        return *this;
    _fd_sock = other._fd_sock;
    _username = other._username;
    _realname = other._realname;
    _nick = other._nick;
    _message = other._message;
    return *this;
}

User::~User() {}

void	User::processIncommingMessage(std::string buf) {
    size_t      pos;
    std::string msg_line;
    std::string cr_lf = "\r\n";

    std::cout << "User <" << _fd_sock << ", " << _username 
            << "> incooming msg(" << buf.size() <<"): "  << buf << std::endl;
    _msg_buf += buf;
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
    
}