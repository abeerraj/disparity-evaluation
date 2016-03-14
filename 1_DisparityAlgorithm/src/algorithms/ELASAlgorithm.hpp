#pragma once

#include "DisparityAlgorithm.hpp"

class ELASAlgorithm : public DisparityAlgorithm {
public:
	ELASAlgorithm(std::string imgL, std::string imgR) : DisparityAlgorithm(imgL, imgR) { }

	void compute();
};
