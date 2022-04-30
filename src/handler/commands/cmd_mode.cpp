#include "../Handler.hpp"

void	Handler::_mode_o(const std::vector<std::string> &param, Channel &channel, User &user, bool flag) {
	std::cout << "mode_o user<" << user.getUsername() << ">" << std::endl;
	if (param.size() < 3) {
		_error_msg(user, 461, "");
		return ;
	}
	std::string new_user = param[2];
	if (!channel._is_user_on_channel(new_user)) {
		_error_msg(user, 401, "");
//		return ; // todo нужен return?
	}
	else if (flag) {
		_write_to_channel(channel.getName(), user, "MODE +o " + new_user);
		channel.setOperators(new_user);
	}
	else if (!flag) {
		_write_to_channel(channel.getName(), user, "MODE -o " + new_user);
		channel.delOperators(new_user);
	}

}

void	Handler::_mode_p(const std::vector<std::string> &param, Channel &channel, User &user, bool flag) {
	if (flag) {
		channel.setFlags(PRIVATE);
		_write_to_channel(channel.getName(), user, "MODE +p");
	}
	else {
		channel.delFlag(PRIVATE);
		_write_to_channel(channel.getName(), user, "MODE -p");
	}
}

void	Handler::_mode_s(const std::vector<std::string> &param, Channel &channel, User &user, bool flag) {
	if (flag) {
		channel.setFlags(SECRET);
		_write_to_channel(channel.getName(), user, "MODE +s");
	}
	else {
		channel.delFlag(SECRET);
		_write_to_channel(channel.getName(), user, "MODE -s");
	}
}

void	Handler::_mode_i(const std::vector<std::string> &param, Channel &channel, User &user, bool flag) {
	if (flag) {
		channel.setFlags(INVITE_ONLY);
		_write_to_channel(channel.getName(), user, "MODE +i");
	}
	else {
		channel.delFlag(INVITE_ONLY);
		_write_to_channel(channel.getName(), user, "MODE -i");
	}
}

void	Handler::_mode_k(const std::vector<std::string> &param, Channel &channel, User &user, bool flag) {
	if (param.size() < 3) {
		_error_msg(user, 461, "");
		return ;
	}
	std::string pass = param[2];
	if (flag && (channel.getFlags() & HAS_PASS))
		_error_msg(user, 467, "");
	else if (flag && !(channel.getFlags() & HAS_PASS)) {
		channel.setFlags(HAS_PASS);
		channel.setPass(pass);
		_write_to_channel(channel.getName(), user, "MODE +k");
	}
	else {
		channel.delFlag(HAS_PASS);
		channel.setPass("");
		_write_to_channel(channel.getName(), user, "MODE -k");
	}
}

void	Handler::_mode_b(const std::vector<std::string> &param, Channel &channel, User &user, bool flag) {
	if (param.size() < 3) {
		_error_msg(user, 461, "");
		return ;
	}
	std::string maska_for_all = "*!*@*";
	std::string new_ban = param[2];
	if (new_ban == maska_for_all && flag) {
		channel.setFlags(BAN_ALL);
		_write_to_channel(channel.getName(), user, "MODE +b" + maska_for_all);
	}
	else if (new_ban == maska_for_all && !flag) {
		channel.delFlag(BAN_ALL);
		_write_to_channel(channel.getName(), user, "MODE -b " + maska_for_all);
	}
	// проверим что юзер, которого хотим забанить на нашем канале
	else if (!channel._is_user_on_channel(new_ban))
		_error_msg(user, 401, "");
	// иначе просто бан на какого-то юзера
	else {
		// записать в бан
		if (flag) {
			channel.setInBanList(new_ban);
			_write_to_channel(channel.getName(), user, "MODE +b " + new_ban);
		}
		// удалить из бана
		else {
			channel.delFromBanList(new_ban);
			_write_to_channel(channel.getName(), user, "MODE -b " + new_ban);
		}
	}
}

void	Handler::_cmd_mode(Message &msg, User &user) {
	std::cout << "cmd_mode " << user.getUsername() << std::endl;
	// недостаточно аргументов
	if (msg.get_params().size() < 2) {
		_error_msg(user, 461, "");
		return;
	}
		// проверка канала на существование
	else if (_server.map_channels.find(msg.get_params()[0]) == _server.map_channels.end()) {
		_error_msg(user, 403, "");
		return;
	}

	std::string name_channel = msg.get_params()[0];
	std::string modes = msg.get_params()[1];
	std::vector<std::string>::const_iterator it;
	// проверка пользователя на оператора канала
	if ( ! (_server.map_channels[name_channel]->_is_user_operator(user.getNick())) ) {
		_error_msg(user, 482, "");
		return ;
	}
	// check на + и -
	else if (modes.size() == 0 || (modes[0] != '+' && modes[0] != '-')) {
//		std::cout << (modes.size() == 0) << (modes[0] != '+' && modes[0] != '-') << std::endl;
		_error_msg(user, 472, "");
		return ;
	}
	else {
		// check на верные буквы для mode
		bool	flag_plus_minus;
		modes[0] == '+' ? flag_plus_minus = true : flag_plus_minus = false;
		modes.erase(0, 1);
//		std::cout << "modes: " << modes <<std::endl;
		for (int i = 0; i < modes.size(); i++) {
			if (_set_modes.count(modes[i]) == 0) {
				_error_msg(user, 472, "");
				return;
			}
		}
		// УРА! ЗАПУСКАЕМ МОДЫ!
		for (int i = 0; i < modes.size(); i++) {
			(this->*_modes[modes[i]])(msg.get_params(), *_server.map_channels[name_channel], user, flag_plus_minus);
		}
	}
}