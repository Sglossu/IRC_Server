#include "server/Server.hpp"


int main(int argc, char** argv)
{
	Server	server(argv[1], std::stoi(argv[2]), "localhost");
	server.init_server();
	try {
		server.start();
	}
	catch (const char *exception){
		std::cerr << "Error: " << exception << std::endl;
	}
	return 0;
}
