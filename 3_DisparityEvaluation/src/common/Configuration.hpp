#pragma once

struct Configuration {
	const std::string left;
	const std::string dispTruthLeft;
	const std::string dispTruthRight;

	Configuration(std::string left, std::string dispTruthLeft, std::string dispTruthRight) :
			left(left), dispTruthLeft(dispTruthLeft), dispTruthRight(dispTruthRight) { }
};
