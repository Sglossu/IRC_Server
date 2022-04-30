#pragma once

#include <string>

#define NOT_SET -1

class UserKey {
    private:
        int         _fd;
        std::string _nick;
    public:
        UserKey(const int fd, const std::string & nick);
        UserKey(const int fd);
        UserKey(const std::string &nick);
        void setFd(const int fd);
        void setNick(const std::string & nick);
        const int getFd() const;
        const std::string & getNick() const;
};

bool operator==(const UserKey &, const UserKey &);
bool operator<(const UserKey &, const UserKey &);