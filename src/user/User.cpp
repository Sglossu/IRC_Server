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
