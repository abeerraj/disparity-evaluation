#pragma once

struct Configuration {
	const std::string dispLeft;
	const std::string dispTruthLeft;
	const std::string bitmask;

	Configuration(std::string dispLeft, std::string dispTruthLeft, std::string bitmask) :
			dispLeft(dispLeft), dispTruthLeft(dispTruthLeft), bitmask(bitmask) { }
};
