#pragma once

#include "DisparityAlgorithm.hpp"

class MRFStereo : public DisparityAlgorithm {
public:
	MRFStereo(std::string imgL, std::string imgR, int MRFalg) : DisparityAlgorithm(imgL, imgR) {
		this->MRFalg = MRFalg;
	}

	void compute();

private:
	int MRFalg;
};
