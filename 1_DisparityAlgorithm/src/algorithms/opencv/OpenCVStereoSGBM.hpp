#pragma once

#include <iostream>
#include "OpenCVStereoMatcher.hpp"

class OpenCVStereoSGBM : public OpenCVStereoMatcher {
public:
	OpenCVStereoSGBM(std::string imgL, std::string imgR) : OpenCVStereoMatcher(imgL, imgR) {
		matL = cv::imread(this->imgL, CV_LOAD_IMAGE_COLOR);
		matR = cv::imread(this->imgR, CV_LOAD_IMAGE_COLOR);

		cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(-48, 128, 5);
		sgbm->setPreFilterCap(63);
		sgbm->setUniquenessRatio(10);
		sgbm->setSpeckleWindowSize(100);
		sgbm->setSpeckleRange(2);
		sgbm->setDisp12MaxDiff(1);
		sgbm->setMode(cv::StereoSGBM::MODE_HH);
		sgbm->setP1(600);
		sgbm->setP2(2400);
		sm = sgbm;
	}
};
