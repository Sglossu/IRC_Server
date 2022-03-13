#include "ServerException.hpp"

ServerException::ServerException (const std::string &msg) : _msg(msg) {}

const char* ServerException::what() const { return _msg.c_str(); }