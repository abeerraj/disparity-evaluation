#pragma once

#include "Constants.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ximgproc/disparity_filter.hpp>

class DisparityAlgorithm {
public:
	virtual void compute() = 0;

	DisparityAlgorithm(std::string imgL, std::string imgR) {
		this->imgL = imgL;
		this->imgR = imgR;
	}

	const cv::Mat &getResult() const {
		return result;
	}

protected:
	std::string imgL, imgR;
	cv::Mat result;
};
