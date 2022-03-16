//#include "irc.hpp"
#include "../../inc/irc.hpp"

void    critErr(std::string msg) {
    std::cerr << msg << std::endl;
    exit(1);
}

int		stringToNumber (std::string &text)
{
	int number;
	if ( ! (std::istringstream(text) >> number) )
		number = 0;
	return number;
}