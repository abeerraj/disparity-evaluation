#include <iostream>
#include <fstream>
#include <opencv2/ximgproc/disparity_filter.hpp>
#include "OpenCVStereoSGBM.hpp"

void OpenCVStereoSGBM::compute() {
	int calculatedNumDisparities = ((imgL.cols / 8) + 15) & -16;
	//Bitweise Maskierung mit -16 beduetet dass die 4 letzten Bits zu 0 werden
	// => die Zahl ist dadurch automatisch eine Vielfache von 16
	calculatedNumDisparities *= 2;

	int minDisparity = -calculatedNumDisparities / 2;
	int numberOfDisparities = calculatedNumDisparities;

	cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(-48, 128, 5);
	sgbm->setPreFilterCap(63);
	sgbm->setUniquenessRatio(10);
	sgbm->setSpeckleWindowSize(100);
	sgbm->setSpeckleRange(2);
	sgbm->setDisp12MaxDiff(1);
	sgbm->setMode(cv::StereoSGBM::MODE_HH);
	sgbm->setP1(600);
	sgbm->setP2(2400);
	//sgbm->compute(imgL, imgR, getResult());

	// WLS filtering with left-to-right / right-to-left cross-checking
	cv::Mat disp, dispR;

	cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter = cv::ximgproc::createDisparityWLSFilter(sgbm);
	cv::Ptr<cv::StereoMatcher> sgbmR = cv::ximgproc::createRightMatcher(sgbm);
	sgbm->compute(imgL, imgR, disp);
	sgbmR->compute(imgR, imgL, dispR);

	cv::Mat filteredDisp;

	wls_filter->setLambda(8000.0);
	wls_filter->setSigmaColor(1.5);
	wls_filter->filter(disp, imgL, filteredDisp, dispR);

	filteredDisp.convertTo(this->result, CV_32F, 1.0 / 16);
}
