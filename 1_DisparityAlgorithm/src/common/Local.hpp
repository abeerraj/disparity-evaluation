#pragma once

#include <map>

class Local {
public:
	void parse(std::istream &cfgfile);

	std::string getOption(std::string id);

private:
	std::vector<std::string> split(std::string str, char delimiter);

	std::map<std::string, std::string> options;
};
