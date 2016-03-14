#pragma once

#include "DisparityAlgorithm.hpp"

class ELASAlgorithm : public DisparityAlgorithm {
public:
	ELASAlgorithm(std::string imgL, std::string imgR) : DisparityAlgorithm(imgL, imgR) { }

	void compute();

	void SwapBytes(unsigned char* pixels, int nPixels, int pixDepth) const;
	void read_comment(FILE *fp) const;
	cv::Mat loadPfm(const std::string filename) const;
};
