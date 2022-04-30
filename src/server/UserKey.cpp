#include "UserKey.hpp"

UserKey::UserKey(const int fd, const std::string & nick) : _fd(fd), _nick(nick) {}

UserKey::UserKey(const int fd) : _fd(fd) {}

UserKey::UserKey(const std::string &nick) : _fd(NOT_SET), _nick(nick) {}

void UserKey::setFd(const int fd) {_fd = fd;}

void UserKey::setNick(const std::string & nick) {_nick = nick;}

const int UserKey::getFd() const {return _fd;}

const std::string & UserKey::getNick() const {return _nick;}

bool operator==(const UserKey & client1, const UserKey & client2) {
    if (client1.getFd() == NOT_SET || client2.getFd() == NOT_SET)
        return client1.getNick() == client2.getNick();
    else
        return client1.getFd() == client2.getFd();
}

bool operator<(const UserKey & client1, const UserKey & client2) {
    if (client1.getFd() == NOT_SET || client2.getFd() == NOT_SET)
        return client1.getNick() < client2.getNick();
    else
        return client1.getFd() < client2.getFd();
}