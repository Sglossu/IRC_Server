#include "../Handler.hpp"

bool	is_channelname_correct(std::string channel) {
	if ((channel[0] != '#' && channel[0] != '&') || channel.size() > 200)
		return false;
	if ( !(channel.find(',') == channel.npos || channel.find(' ') == channel.npos))
		return false;
	return true;
}

void	Handler::_delete_nick_from_setnicks(Channel &channel) {
	for (int i = 0; i < channel.getUsers().size(); i++) {
		_set_nicks.erase(channel.getUsers()[i]);
	}
}

void	Handler::_make_nicks_set() {
	_set_nicks.clear();
	std::map<std::string, User *>::iterator it_begin = _server.mapnick_users.begin();
	std::map<std::string, User *>::iterator it_end = _server.mapnick_users.end();
	while (it_begin != it_end) {
		_set_nicks.insert(it_begin->second->getNick());
		it_begin++;
	}
}

void	Handler::_cmd_join(Message &msg, User &user) {
	std::cout << "cmd_join " << user.getUsername() << std::endl;

	// todo префикс нужен или пофиг? - пофик (Саша)
	if (!msg.get_params().size())
		_error_msg(user, 461, "JOIN");

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
		size_t					i = 0;
		while (std::getline(ss_pass, item, sep) && i < vector_arg.size())
		{
			arg[vector_arg[i]] = item;
			i++;
		}
	}
	// отдаём каждый канал классу Channel для обработки
	for (size_t i = 0; i < vector_arg.size(); i++) {
		if (!is_channelname_correct(vector_arg[i]))
			_error_msg(user, 403, vector_arg[i]);
		else {
			if (!_is_channel_exist(vector_arg[i]))
				// если канала ещё нет, создаём
				_server.map_channels[vector_arg[i]] = new Channel(vector_arg[i], user, arg[vector_arg[i]], this);

			_server.map_channels[vector_arg[i]]->_join_user(user, arg[vector_arg[i]], false);
		}
	}
}

void	Handler::_cmd_part(Message &msg, User &user) {
	std::cout << "cmd_part " << user.getUsername() << std::endl;

	// todo префикс нужен или пофиг?
	if (!msg.get_params().size())
		_error_msg(user, 461, "PART");

	std::stringstream						ss_name(msg.get_params()[0]);
	std::string								item;
	char									sep = ',';
	std::vector<std::string>				channel_names;

	// записываем в мапу названия каналов
	while (std::getline(ss_name, item, sep))
		channel_names.push_back(item);

	// отдаём каждый канал классу Channel для обработки
	for (size_t i = 0; i < channel_names.size(); i++) {
		if (!_server.map_channels.count(channel_names[i])) {
			_error_msg(user, 403, channel_names[i]);
			continue;
		}
		Channel *current_channel = _server.map_channels[channel_names[i]];
		if (!current_channel->_is_user_on_channel(user.getNick()))
			_error_msg(user, 442, channel_names[i]);
		else {
			_write_to_channel(channel_names[i], user, "PART " + channel_names[i]);
			current_channel->_delete_user(user.getNick());
		}

	}
}

void	Handler::_cmd_invite(Message &msg, User &user) {
//	Параметры: <nickname> <channel>

	std::cout << "cmd_invite " << user.getUsername() << std::endl;
	if (msg.get_params().size() < 2)
		_error_msg(user, 461, "INVITE");
	// канала нет
	else if (!_is_channel_exist(msg.get_params()[1])) {
		_error_msg(user, 403, msg.get_params()[1]);
	}
	// приглашает неизвестного
	else if (!_is_nick_exist(msg.get_params()[0])) {
		_error_msg(user, 401, msg.get_params()[0]);
	}
	// пригласивший сам не на канале
	else if (!_server._is_user_on_channel(msg.get_params()[1], user.getNick()))
		_error_msg(user, 442, msg.get_params()[1]);
	// если чел, кого приглашают, уже на канале - обрабатывается в канале в join user
	// пригласивший - не оператор
	else if (!_server.map_channels[msg.get_params()[1]]->_is_user_operator(user.getNick()))
		_error_msg(user, 482, msg.get_params()[1]);
	// else DONE! подключить к каналу)
	else {
		_cmd_responses(msg.get_params()[1] + " " + msg.get_params()[0], user, 341);
		_server.map_channels[msg.get_params()[1]]->
			_join_user(*_server.mapnick_users[msg.get_params()[0]], "", true);
		std::string nick = msg.get_params()[0];
		std::string ms = prefix_msg(user) + "INVITE " + nick + " :"+ msg.get_params()[1] + CR_LF;
		_write_to_channel(msg.get_params()[1], user, ms);
	}

}

void	Handler::_cmd_kick(Message &msg, User &user) {
//	Параметры: <channel> <user> [<comment>]
// не добавляла MODE +-t, то есть топик может ставить кто хочет
	std::cout << "cmd_kick " << user.getUsername() << std::endl;
	if (msg.get_params().size() < 2)
		return _error_msg(user, 461, "KICK");

	std::string channel_name(msg.get_params()[0]);
	if (!_server.map_channels.count(channel_name))
		return _error_msg(user, 403, channel_name);

	Channel *channel = _server.map_channels[channel_name];
	
	if (!channel->_is_user_on_channel(user.getNick()))
		return _error_msg(user, 442, channel_name);
	
	if (!channel->_is_user_operator(user.getNick()))
		return _error_msg(user, 482, channel_name);

	std::stringstream			ss_nicks(msg.get_params()[1]);
	std::string					nick;
	// std::vector<std::string>	nicks;
	// записываем в вектор пользователей
	while (std::getline(ss_nicks, nick, ',')) {
		if (!channel->_is_user_on_channel(nick)) {
			_error_msg(user, 441, nick + " " + channel_name);
			continue;
		}
		std::string kick_msg = "KICK " + channel_name + " " + nick + " :";
		if (msg.get_params().size() == 3)
			kick_msg += msg.get_params()[2];
		else
			kick_msg += "Because i can";
		_write_to_channel(channel_name, user, kick_msg);
		channel->_delete_user(nick);
	}
	

}

void	Handler::_cmd_topic(Message &msg, User &user) {
	std::cout << "cmd_topic " << user.getUsername() << std::endl;
	if (msg.get_params().size() < 1) {
		_error_msg(user, 461, "TOPIC");
//		return ;
	}
	// check что пользователь на канале
	else if (_server.map_channels.find(msg.get_params()[0]) == _server.map_channels.end() ||
			!_server.map_channels[msg.get_params()[0]]->_is_user_on_channel(user.getNick())) {
		_error_msg(user, 442, msg.get_params()[0]);
	}
	else {
		Channel *channel = _server.map_channels[msg.get_params()[0]];
		// если только 2 аргумента - возвращаем топик
		if (msg.get_params().size() == 1) {
			channel->getTopic().empty() ?
				_cmd_responses(channel->getName(), user, 331) :
					_cmd_responses(channel->getName() + " :" + channel->getTopic(), user, 332);
		}
		// если 3 аргумента - устанавливаем топик
		else {
			channel->setTopic(msg.get_params()[1]);
			std::string message = "TOPIC " + channel->getName() + " :" + msg.get_params()[1];
			_write_to_channel(channel->getName(), user, message);
		}
	}
}

void	Handler::_cmd_names(Message &msg, User &user) {
	std::cout << "cmd_names " << user.getUsername() << std::endl;
	// only NAMES
	if (msg.get_params().size() == 0) {
		// вывод пользователей в каналах
		_make_nicks_set();

		std::map<std::string, Channel *>::iterator it;
		for (it = _server.map_channels.begin(); it != _server.map_channels.end(); it++)
		{
			_delete_nick_from_setnicks(*it->second);
			if (it->second->getFlags() & PRIVATE || it->second->getFlags() & SECRET)
				continue;
			std::string names = it->second->_namreply(user);
			_cmd_responses(names, user, 353);
		}
		// собираем всех оставшихся юзеров
		std::string 	remaining_users;
		std::set<std::string>::iterator itr;
		for (itr = _set_nicks.begin(); itr != _set_nicks.end(); itr++) {
			remaining_users += *itr + " ";
		}
		_cmd_responses(user.getNick() + " * * :" + remaining_users, user, 353);
		// end of names
		_cmd_responses("", user, 366);
	}
	// NAMES и параметры
	else {
		std::vector<std::string> channels = msg.get_params();
		for (int i = 0; i < channels.size(); i++)
		{
			if (_is_channel_exist(channels[i]))
			{
				std::string names = _server.map_channels[channels[i]]->_namreply(user);
				_cmd_responses(names, user, 353);
			}
		}
		_cmd_responses(user.getNick(), user, 366);
	}
}

template<class InputIt1, class InputIt2,
		class OutputIt, class Compare>
OutputIt set_difference( InputIt1 first1, InputIt1 last1,
						 InputIt2 first2, InputIt2 last2,
						 OutputIt d_first, Compare comp)
{
	while (first1 != last1) {
		if (first2 == last2) return std::copy(first1, last1, d_first);

		if (comp(*first1, *first2)) {
			*d_first++ = *first1++;
		} else {
			if (!comp(*first2, *first1)) {
				++first1;
			}
			++first2;
		}
	}
	return d_first;
}