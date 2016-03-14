#pragma once

#include "OpenCVStereoMatcher.hpp"

class OpenCVStereoBM : public OpenCVStereoMatcher {
public:
	OpenCVStereoBM(std::string imgL, std::string imgR) : OpenCVStereoMatcher(imgL, imgR) {
		matL = cv::imread(this->imgL, CV_LOAD_IMAGE_GRAYSCALE);
		matR = cv::imread(this->imgR, CV_LOAD_IMAGE_GRAYSCALE);

		cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(128, 9);
		sbm->setPreFilterSize(5);
		sbm->setPreFilterCap(61);
		sbm->setMinDisparity(-48);
		sbm->setTextureThreshold(507);
		sbm->setUniquenessRatio(0);
		sbm->setSpeckleWindowSize(0);
		sbm->setSpeckleRange(8);
		sbm->setDisp12MaxDiff(1);
		sm = sbm;
	}
};
