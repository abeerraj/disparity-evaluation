#pragma once

#include "DisparityAlgorithm.hpp"

class OpenCVStereoMatcher : public DisparityAlgorithm {
public:
	OpenCVStereoMatcher(std::string imgL, std::string imgR) : DisparityAlgorithm(imgL, imgR) { }

	void compute(const int identifier) {
		cv::Mat disp, dispR;

		cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter = cv::ximgproc::createDisparityWLSFilter(sm);
		cv::Ptr<cv::StereoMatcher> smR = cv::ximgproc::createRightMatcher(sm);
		sm->compute(matL, matR, disp);
		smR->compute(matR, matL, dispR);

		cv::Mat filteredDisp;
		wls_filter->setLambda(8000.0);
		wls_filter->setSigmaColor(1.5);
		wls_filter->filter(disp, matL, filteredDisp, dispR);
		filteredDisp.convertTo(this->result, CV_32F, 1.0 / 16);
	}

protected:
	cv::Ptr<cv::StereoMatcher> sm;
	cv::Mat matL, matR;
};
