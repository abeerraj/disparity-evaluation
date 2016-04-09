#pragma once

#include <vector>
#include <map>
#include <mutex>

class Properties {
public:
	Properties(std::string filename);

	std::string getProperty(std::string id);

private:
	std::mutex _mutex;

	void parse(std::ifstream &localfile);

	std::vector<std::string> tokenize(std::string str, char delimiter);

	static std::map<std::string,std::string> & GetPropertiesMap() {
		static std::map<std::string, std::string> properties;
		return properties;
	};

};
