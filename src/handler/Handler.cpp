#include "Handler.hpp"

Handler::Handler(Server &server): _server(server) {
	_registration_commands();
	_registration_modes();
	char tmp[] = {'p', 'o', 's', 'i', 't', 'n', 'm', 'l', 'b', 'v', 'k'};
	int tmp_size = 11;
	for (int i = 0; i < tmp_size; i++)
		_set_modes.insert(tmp[i]);
	// std::cout << "handler constructor. server port: " << server.port << std::endl;
}

Handler::~Handler() {}

void	Handler::process_incomming_message(int fd, std::string buf) {
	size_t      pos;
	std::string msg_line;
	User		*user;

	if (_bufs.count(fd))
		_bufs[fd] += buf;
	else
		_bufs[fd] = buf;
	user = _server.mapfd_users[fd];
	std::cout << "User <" << fd << ", " << user->getUsername()
			  << "> incoming msg(" << _bufs[fd].size() <<"): "  << _bufs[fd] << std::endl;
	while (!_bufs[fd].empty()) {
		pos = _bufs[fd].find(CR_LF);
		if (pos == _bufs[fd].npos)
			break ;
		msg_line = _bufs[fd].substr(0, pos);
		msg_line = msg_line.substr(0, msg_line.find(CR_LF));

		_bufs[fd] = _bufs[fd].substr(pos + strlen(CR_LF));

		Message	msg(msg_line);
		if (msg.get_cmd().empty() or !_commands.count(msg.get_cmd())) {
			std::cout << "|!Unknown command: |" << msg.get_cmd() << std::endl;
			_error_msg(*user, 421);
			continue ;
		}

		// Самая Классная Строчка
		// Запускает нужную команду. Синтаксис пиздец...
		if ((user->get_flags() & REGISTERED))
			(this->*_commands[msg.get_cmd()])(msg, *user);
		else if (!check_registration(&msg, *user))
			continue ;
		else
			(this->*_commands[msg.get_cmd()])(msg, *user);
		// если пользователь заполнил все поля для регистрации
		if (!(user->get_flags() & REGISTERED) &&
			(user->get_flags() & ENTER_NAME) &&
			(user->get_flags() & ENTER_NICK) &&
			(user->get_flags() & ENTER_NAME)) {
				user->set_flag(REGISTERED);
				user->set_flag(PRINT_MOTD);
				_server.mapnick_users[user->getNick()] = user;
				std::cout << "<User: fd " << user->getFdSock() << "> has been registered" << std::endl;
		}
		if (user->get_flags() & PRINT_MOTD) {
			_cmd_msgoftheday(*user);
			user->del_flag(PRINT_MOTD);
		}
	}
}

bool	Handler::check_registration(Message *msg, User &user) {

	if (!msg->get_cmd().compare("PASS"))
		return true;
	if (!msg->get_cmd().compare("USER") || !msg->get_cmd().compare("NICK"))
		if (!(user.get_flags() & ENTER_PASS)) {
			_error_msg(user, 502);
			return false;
		}
	if (msg->get_cmd().compare("USER") && msg->get_cmd().compare("NICK")
		&& msg->get_cmd().compare("PASS")) {
		if (!(user.get_flags() & ENTER_PASS) || !(user.get_flags() & ENTER_NAME) || !(user.get_flags() & ENTER_NICK)) {
				_error_msg(user, 451);
				return false;
			}
		}
	return true;
}

// проверка существования канала
bool	Handler::_is_channel_exist(std::string name_channel) {
	if (_server.map_channels.find(name_channel) != _server.map_channels.end())
		return true;
	return false;
}

// проверка существования ника вообще на всём сервере
bool	Handler::_is_nick_exist(std::string nick) {
	if (_server.mapnick_users.find(nick) != _server.mapnick_users.end())
		return true;
	return false;
}

void	Handler::_write_to_channel(std::string name_channel, User &user, std::string msg) {
	std::string nick_user = user.getNick();
	std::string preview = ":" + nick_user + "!" + nick_user + "@" + _host + " ";
	std::string res = preview + msg + "\r\n";

	std::vector<std::string>  users = _server.map_channels[name_channel]->getUsers();
	for (size_t i = 0; i < users.size(); i++)
		_server.write_to_client(users[i], res);
}

bool	Handler::_is_valid_nick(std::string	nick) {
	// todo
	return true;
}

bool	Handler::_is_valid_channelname(std::string name) {
	// todo
	return true;
}

void	Handler::clear_buf(int fd) {
	if (_bufs.count(fd))
		_bufs.erase(fd);
}

const YamlParser &Handler::getConfig() const {
	return _config;
}


void Handler::setHost(const std::string &host) {
	_host = host;
}