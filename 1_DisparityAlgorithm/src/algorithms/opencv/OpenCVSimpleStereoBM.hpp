#pragma once

#include "DisparityAlgorithm.hpp"

class OpenCVSimpleStereoBM : public DisparityAlgorithm {
public:
	OpenCVSimpleStereoBM(std::string imgL, std::string imgR) : DisparityAlgorithm(imgL, imgR) { }

	void compute(const int identifier) {
		matL = cv::imread(this->imgL, CV_LOAD_IMAGE_GRAYSCALE);
		matR = cv::imread(this->imgR, CV_LOAD_IMAGE_GRAYSCALE);

		//cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(128, 9);
		cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(64, 9);
		sbm->setPreFilterSize(5);
		sbm->setPreFilterCap(61);
		//sbm->setMinDisparity(-48);
		sbm->setMinDisparity(0);
		sbm->setTextureThreshold(507);
		sbm->setUniquenessRatio(0);
		sbm->setSpeckleWindowSize(0);
		sbm->setSpeckleRange(8);
		sbm->setDisp12MaxDiff(1);

		cv::Mat dispL;
		sbm->compute(matL, matR, dispL);
		dispL.convertTo(this->result, CV_32F, 1.0 / 16);
	}

private:
	cv::Mat matL, matR;
};
