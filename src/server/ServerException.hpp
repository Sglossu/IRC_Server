#pragma once

#include <string>

class ServerException {
    private:
        std::string _msg;
    public:
        ServerException (const std::string&);
        virtual const char* what() const throw();
};