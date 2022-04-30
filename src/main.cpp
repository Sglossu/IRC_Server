#include "../inc/irc.hpp"
#include "server/Server.hpp"
#include "user/User.hpp"
#include "handler/Handler.hpp"


int main(int argc, char** argv)
{
	if (argc != 3 and argc != 4)
		critErr(INCORRECT_ARGV);

	Server	server(argv[argc - 2], argv[argc - 1], "localhost");
	server.init_server();

	try {
		server.start();
//		handler.getPserver()->start();
	}
	catch (const char *exception) {
		critErr("Error: " + std::string(exception));
	}
	return 0;
}
