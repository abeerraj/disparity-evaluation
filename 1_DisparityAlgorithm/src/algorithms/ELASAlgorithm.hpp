#pragma once

#include "DisparityAlgorithm.hpp"

class ELASAlgorithm : public DisparityAlgorithm {
public:
	ELASAlgorithm(std::string imgL, std::string imgR) {
		this->imgL = imgL;
		this->imgR = imgR;
	}

	void compute();

private:
	std::string imgL, imgR;
};
