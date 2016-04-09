#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Properties.hpp"
#include "Constants.hpp"

using namespace std;

map<string, string> Properties::properties;

Properties::Properties(string filename) {
	unique_lock<mutex> lock(_mutex);
	if (!Properties::properties.empty()) return;
	ifstream cfg(filename);
	parse(cfg);
}

vector<string> Properties::tokenize(string str, char delimiter) {
	vector<string> tokenized;
	stringstream ss(str);
	for (string token; getline(ss, token, delimiter);) {
		tokenized.push_back(token);
	}
	return tokenized;
}

void Properties::parse(istream &localfile) {
	for (string line; getline(localfile, line);) {
		line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
		vector<string> token = tokenize(line, '=');
		if (token.size() != 2) continue;
		string id = token[0];
		string val = token[1];
		Properties::properties[id] = val;
		if (Constants::debug) cout << "option['" << id << "'] = '" << val << "'" << endl;
	}
}

string Properties::getProperty(string id) {
	return Properties::properties[id];
}
