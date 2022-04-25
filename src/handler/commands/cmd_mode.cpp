#include "../Handler.hpp"

void	Handler::_mode_o(const std::vector<std::string> &param, Channel &channel, User &user, bool flag) {
	std::cout << "mode_o user<" << user.getUsername() << ">" << std::endl;
	if (param.size() < 3) {
		_error_msg(user, 461);
		return ;
	}
	std::string new_user = param[2];
	if (!channel._is_user_on_channel(new_user)) {
		_error_msg(user, 401);
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

void	Handler::_cmd_mode(Message &msg, User &user) {
	std::cout << "cmd_mode " << user.getUsername() << std::endl;
	// недостаточно аргументов
	if (msg.get_params().size() < 2) {
		_error_msg(user, 461);
		return;
	}
		// проверка канала на существование
	else if (_server.map_channels.find(msg.get_params()[0]) == _server.map_channels.end()) {
		_error_msg(user, 403);
		return;
	}

	std::string name_channel = msg.get_params()[0];
	std::string modes = msg.get_params()[1];
	std::vector<std::string>::const_iterator it;
	// проверка пользователя на оператора канала
	if ( ! (_server.map_channels[name_channel]->_is_user_operator(user.getNick())) ) {
		_error_msg(user, 482);
		return ;
	}
	// check на + и -
	else if (modes.size() == 0 || (modes[0] != '+' && modes[0] != '-')) {
//		std::cout << (modes.size() == 0) << (modes[0] != '+' && modes[0] != '-') << std::endl;
		_error_msg(user, 472);
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
				_error_msg(user, 472);
				return;
			}
		}
		// УРА! ЗАПУСКАЕМ МОДЫ!
		for (int i = 0; i < modes.size(); i++) {
			(this->*_modes[modes[i]])(msg.get_params(), *_server.map_channels[name_channel], user, flag_plus_minus);
		}
	}
}