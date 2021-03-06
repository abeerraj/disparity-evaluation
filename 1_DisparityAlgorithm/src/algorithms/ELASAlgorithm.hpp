#pragma once

#include "DisparityAlgorithm.hpp"
#include "LocalConfig.hpp"

class ELASAlgorithm : public DisparityAlgorithm {
public:
	ELASAlgorithm(std::string imgL, std::string imgR) : DisparityAlgorithm(imgL, imgR) {

	}

	void compute(const int identifier);

private:
	LocalConfig localConfig;
};
