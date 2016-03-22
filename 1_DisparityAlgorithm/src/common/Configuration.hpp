#pragma once

struct Configuration {
	const int identifier;
	const int algorithmId;
	const std::string left;
	const std::string right;
	const std::string out;

	Configuration(int identifier, int algorithmId, std::string left, std::string right, std::string out) :
			identifier(identifier), algorithmId(algorithmId), left(left), right(right), out(out) { }
};
