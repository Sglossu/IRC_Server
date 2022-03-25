#pragma once
#ifndef YAML_PARSER_HPP
#define YAML_PARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

class YamlParser {
	std::ifstream file;
	std::map<std::string, std::string> mapConfig;
public:
	YamlParser();
	virtual ~YamlParser();
	void loadFile();
	void reload();
	std::string operator [](std::string const& name);
	std::string const operator [](std::string const& name) const;
};

#endif