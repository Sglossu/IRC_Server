#pragma once

#include <string>

class ServerException : public std::exception {
    private:
        std::string _msg;
    public:
        ServerException (const std::string&);
        virtual const char* what() const throw();
};