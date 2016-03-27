#include <iostream>
#include <sstream>
#include <vector>
#include "Local.hpp"

using namespace std;

vector<string> Local::split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str);
	for (string tok; getline(ss, tok, delimiter);) {
		internal.push_back(tok);
	}
	return internal;
}

void Local::parse(istream &cfgfile) {
	for (string line; getline(cfgfile, line);) {
		vector<string> token = split(line, '=');
		if (token.size() != 2) continue;
		string id = token[0];
		string val = token[1];
		options[id] = val;
		cout << "option['" << id << "'] = " << val << endl;
	}
}

string Local::getOption(string id) {
	return options[id];
}
