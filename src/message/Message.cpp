#include "Message.hpp"

Message::Message(std::string line)
{
	std::stringstream	ss(line);
    std::string			item;
	char				sep = ' ';
	
	if (line.size() and line[0] == ':') {
		std::getline(ss, item, sep);
		_prefix = item.substr(1);
	}
	std::getline(ss, _cmd, sep);
	while (std::getline(ss, item, sep)) {
		if (item[0] == ':') {
			std::string		tail;
			std::getline(ss, tail, '\n');
			item = item.substr(1);
			if (!tail.empty())
				item = item + " " + tail;
			ss.clear();
		}
		_params.push_back(item);
	}

//	std::cout << "Init msg\n| prefix\t" << _prefix << "\n| cmd   \t" << _cmd <<"\n| params\t" << _params.size() << std::endl;
}

Message::~Message() {}

const std::string &Message::get_prefix() const { return _prefix; }

const std::string &Message::get_cmd() const { return _cmd; }

const std::vector<std::string> &Message::get_params() const { return _params; }
