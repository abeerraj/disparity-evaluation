#pragma once

struct Configuration {
	const std::string left;
	const std::string dispLeft;
	const std::string dispTruthLeft;
	const std::string dispTruthRight;

	Configuration(std::string left, std::string dispLeft, std::string dispTruthLeft, std::string dispTruthRight) :
			left(left), dispLeft(dispLeft), dispTruthLeft(dispTruthLeft), dispTruthRight(dispTruthRight) { }
};
