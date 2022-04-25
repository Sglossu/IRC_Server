#include "../Handler.hpp"

void	Handler::_mode_o() {
	return;
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
	// проверка пользователя на оператора канала
	std::string name_channel = msg.get_params()[0];
	std::string modes = msg.get_params()[1];
	std::vector<std::string>::const_iterator it;
	it = std::find(_server.map_channels[name_channel]->getOperators().begin(),
				   _server.map_channels[name_channel]->getOperators().end(), user.getNick());
	// check является ли пользователь оператором канала
	if (it == _server.map_channels[name_channel]->getOperators().end()) {
		_error_msg(user, 482);
		return ;
	}
	// check на + и -
	else if (modes.size() == 0 || (modes[0] != '+' && modes[0] != '-')) {
		std::cout << (modes.size() == 0) << (modes[0] != '+' && modes[0] != '-') << std::endl;
		_error_msg(user, 472);
		return ;
	}
	else {
		// check на верные буквы для mode
		bool	flag_plus_minus;
		modes[0] != '+' ? flag_plus_minus = true : flag_plus_minus = false;
		modes.erase(0, 1);
		std::cout << "modes: " << modes <<std::endl;
		for (int i = 0; i < modes.size(); i++) {
			if (_set_modes.count(modes[i]) == 0) {
				_error_msg(user, 472);
				return;
			}
		}
	}

}