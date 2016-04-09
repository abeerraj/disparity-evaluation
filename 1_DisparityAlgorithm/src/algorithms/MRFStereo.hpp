#pragma once

#include "DisparityAlgorithm.hpp"
#include "LocalConfig.hpp"

class MRFStereo : public DisparityAlgorithm {
public:
	MRFStereo(std::string imgL, std::string imgR, int MRFalg) : DisparityAlgorithm(imgL, imgR) {
		this->MRFalg = MRFalg;
	}

	void compute(const int identifier);

private:
	int MRFalg;
	LocalConfig localConfig;
};
