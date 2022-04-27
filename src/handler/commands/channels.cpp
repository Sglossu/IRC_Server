#include "../Handler.hpp"

bool	is_channelname_correct(std::string channel) {
	if ((channel[0] != '#' && channel[0] != '&') || channel.size() > 200)
		return false;
	if ( !(channel.find(',') == channel.npos || channel.find(' ') == channel.npos))
		return false;
	return true;
}

void	Handler::_cmd_join(Message &msg, User &user) {
	std::cout << "cmd_join " << user.getUsername() << std::endl;

	// todo префикс нужен или пофиг?
	if (!msg.get_params().size())
		_error_msg(user, 461);

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
			_error_msg(user, 403);
		else {
			if (!_is_channel_exist(vector_arg[i]))
				// если канала ещё нет, создаём
				_server.map_channels[vector_arg[i]] = new Channel(vector_arg[i], user, arg[vector_arg[i]], this);

			_server.map_channels[vector_arg[i]]->_join_user(user, arg[vector_arg[i]], false);
		}
	}
}

void	Handler::_cmd_invite(Message &msg, User &user) {
//	Параметры: <nickname> <channel>

	if (msg.get_params().size() < 2)
		_error_msg(user, 461);
	// канала нет
	else if (!_is_channel_exist(msg.get_params()[1])) {
		_error_msg(user, 401);
	}
	// приглашает неизвестного
	else if (!_is_nick_exist(msg.get_params()[0])) {
		_error_msg(user, 401);
	}
	// пригласивший сам не на канале
	else if (!_server._is_user_on_channel(msg.get_params()[1], user.getNick()))
		_error_msg(user, 442);
	// если чел, кого приглашают, уже на канале - обрабатывается в канале в join user
	// пригласивший - не оператор
	else if (!_server.map_channels[msg.get_params()[1]]->_is_user_operator(user.getNick()))
		_error_msg(user, 482);
	// else DONE! подключить к каналу)
	else {
		_cmd_responses(msg.get_params()[1] + " " + msg.get_params()[0], user, 341);
		_server.map_channels[msg.get_params()[1]]->
			_join_user(*_server.mapnick_users[msg.get_params()[0]], "", true);
		std::string nick = msg.get_params()[0];
		std::string ms = user.getNick() + "!" + user.getNick() + "@" + _server.getLastIpstr() + " INVITE " + nick + " :"+ msg.get_params()[1] + CR_LF;
		_write_to_channel(msg.get_params()[1], user, ms);
	}

}

void	Handler::_cmd_kick(Message &msg, User &user) {
//	Параметры: <channel> <user> [<comment>]
	return ;
}

