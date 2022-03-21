#pragma once
#include "../../inc/irc.hpp"
// #include "Group.hpp"

//#define CR_LF	"\r\n"
#define CR_LF	"\n"
#define REGISTERED		0b00000001
#define ADMIN			0b00000010
#define DISCONNECTED	0b10000000


class User {
private:
	const int		_fd_sock;
	// const Server    _server;
	std::string		_msg_buf;
	std::string		_username;
	std::string		_hostname;
	std::string		_servername;
	std::string		_realname;
	std::string		_nick;
	std::string		_message;
	std::string		_pass;
	unsigned char	_flags;
	bool 			_enter_pass;
	bool 			_enter_nick;
	bool 			_enter_name;
//	std::vector<>	_chanels; // придумать какой тип данных. массив для каналов юзера
	User();
	User(const User &other);
	User& operator=(const User& other);

public:
	User(int fd_sock);
	~User();

	const std::string		&getUsername() const;
	void 					setUsername(const std::string &username);
	const 					std::string &getHostname() const;
	void					setHostname(const std::string &hostname);
	const std::string		&getServername() const;
	void					setServername(const std::string &servername);
	const std::string		&getRealname() const;
	void					setRealname(const std::string &realname);

	void					set_flag(unsigned char flag);
	void					join_group(std::string &group_name);
	const unsigned char		get_flags() const;
	const int 				getFdSock() const;
	const std::string 		&getNick() const;
	void 					setNick(const std::string &nick);

	bool 					isEnterPass() const;
	bool 					isEnterNick() const;

	const std::string 		&getPass() const;
	void 					setPass(const std::string &pass);

	bool 					isEnterName() const;
	void 					setEnterPass(bool enterPass);
	void 					setEnterNick(bool enterNick);
	void 					setEnterName(bool enterName);
};


