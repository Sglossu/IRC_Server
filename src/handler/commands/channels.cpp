#include "../Handler.hpp"

bool	is_channelname_correct(std::string channel) {
	if ((channel[0] != '#' && channel[0] != '&') || channel.size() > 200)
		return false;
	if ( !(channel.find(',') == channel.npos || channel.find(' ') == channel.npos))
		return false;
	return true;
}

bool	Handler::is_channel_exist(std::string name_channel) {
	for (std::map<std::string, Channel *>::iterator it = _server.map_channels.begin(); it != _server.map_channels.end(); it++)
	{
		if (!it->second->getName().compare(name_channel))
			return true;
	}
	return false;
}

void	Handler::_cmd_join(Message &msg, User &user) {
	std::cout << "cmd_join " << user.getUsername() << std::endl;

	if (!msg.get_params().size())
		_error_msg(&user, 461);

	std::stringstream						ss_name(msg.get_params()[0]);
	std::string								item;
	char									sep = ',';
	std::map<std::string, std::string>		arg;
	std::vector<std::string>				vector_arg;

	// записываем в мапу названия каналов
	while (std::getline(ss_name, item, sep)) {
		arg[item] = "";
		vector_arg.push_back(item);
	}
	// если есть пароли, записываем их тоже в мапу подряд к каналам
	if (msg.get_params().size() > 1)
	{
		std::stringstream		ss_pass(msg.get_params()[1]);
		int						i = 0;
		while (std::getline(ss_pass, item, sep) && i < vector_arg.size())
		{
			arg[vector_arg[i]] = item;
			i++;
		}
	}
	// отдаём каждый канал классу Channel для обработки
	for (int i = 0; i < vector_arg.size(); i++) {
		if (!is_channelname_correct(vector_arg[i]))
			_error_msg(&user, 403);
		else {
			if (!is_channel_exist(vector_arg[i]))
				// если канала ещё нет, создаём
				_server.map_channels[user.getUsername()] = new Channel(vector_arg[i], user, arg[vector_arg[i]], this);
			else
				_server.map_channels[user.getUsername()]->_join_user(&user, arg[vector_arg[i]]);
		}
	}


//	if (!is_channelname_correct(item_name))
//		_error_msg(&msg, &user, 403);
//	_server.getMapChannels()[user.getFdSock()] = new Channel(, user);
}