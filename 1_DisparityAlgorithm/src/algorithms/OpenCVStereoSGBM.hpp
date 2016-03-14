#pragma once

#include "DisparityAlgorithm.hpp"

class OpenCVStereoSGBM : public DisparityAlgorithm {
public:
	OpenCVStereoSGBM(cv::Mat imgL, cv::Mat imgR) {
		this->imgL = imgL;
		this->imgR = imgR;
	}

	void compute();

private:
	cv::Mat imgL, imgR;
};
