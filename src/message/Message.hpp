#pragma once
#include "../../inc/irc.hpp"

/*
<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
<prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
<command>  ::= <letter> { <letter> } | <number> <number> <number>
<SPACE>    ::= ' ' { ' ' }
<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]

<middle>   ::= <Any *non-empty* sequence of octets not including SPACE
			   or NUL or CR or LF, the first of which may not be ':'>
<trailing> ::= <Any, possibly *empty*, sequence of octets not including
				 NUL or CR or LF>

*/

class Message {
private:
	std::string					_prefix;
	std::string					_cmd;
	std::vector<std::string>	_params;

	Message();
	Message(const Message &other);
	Message& operator=(const Message& other);

public:
	const std::string				&get_prefix() const;
	const std::string				&get_cmd() const;
	const std::vector<std::string>	&get_params() const;

	Message(std::string line);
	~Message();
};