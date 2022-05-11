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
	int 		len_cr_cn = 2;

	if (_bufs.count(fd))
		_bufs[fd] += buf;
	else
		_bufs[fd] = buf;
	user = _server.mapfd_users[fd];
	if (DEBUG)
	std::cout << GREEN << "\nUser <" << fd << ", " << user->getUsername()
			  << "> incoming msg(" << _bufs[fd].size() <<"): "  << _bufs[fd] << RESET;
	while (!_bufs[fd].empty()) {
		pos = _bufs[fd].find(CR_LF);
		if (pos == _bufs[fd].npos) {
			pos = _bufs[fd].find(CR);
			len_cr_cn = 1;
			if (pos == _bufs[fd].npos)
				break;
		}

		msg_line = _bufs[fd].substr(0, pos);
		len_cr_cn == 2 ?
			msg_line = msg_line.substr(0, msg_line.find(CR_LF)) :
				msg_line = msg_line.substr(0, msg_line.find(CR));

		_bufs[fd] = _bufs[fd].substr(pos + len_cr_cn);

		Message	msg(msg_line);
		if (msg.get_cmd().empty() or !_commands.count(msg.get_cmd())) {
			if (DEBUG)
				std::cout << RED << "|!Unknown command: |" << msg.get_cmd() << RESET << std::endl;
			_error_msg(*user, 421, msg.get_cmd());
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
			_error_msg(user, 1001, ":Password is not entered");
			user.set_flag(DISCONNECTED);
			return false;
		}
	if (msg->get_cmd().compare("USER") && msg->get_cmd().compare("NICK")
		&& msg->get_cmd().compare("PASS")) {
		if (!(user.get_flags() & ENTER_PASS) || !(user.get_flags() & ENTER_NAME) || !(user.get_flags() & ENTER_NICK)) {
				_error_msg(user, 451, "");
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

std::string Handler::prefix_msg(const User &user) {
	std::string prefix = ":" + user.getNick() + "!" + user.getUsername() + "@" + _host + " ";
	return prefix;
}

// send message to all users in channel
void	Handler::_write_to_channel(std::string name_channel, User &user, std::string msg) {
	std::string res = prefix_msg(user) + msg + CR_LF;

	std::vector<std::string>  users = _server.map_channels[name_channel]->getUsers();
	for (size_t i = 0; i < users.size(); i++)
		_server.write_to_client(users[i], res);
}

// send complited msg to whole channel, exclude the owner message
void	Handler::_write_to_channel(Channel &channel, const std::string &complite_msg, const std::string &exclude_nick) {

	std::vector<std::string>  users = channel.getUsers();
	for (size_t i = 0; i < users.size(); i++)
		// if (users[i] != exclude_nick)
		_server.write_to_client(users[i], complite_msg);
}

;
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