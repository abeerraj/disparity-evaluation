#include <iostream>
#include <fstream>
#include "OpenCVStereoSGBM.hpp"

void OpenCVStereoSGBM::compute() {
	cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(-64, 192, 5);

	int calculatedNumDisparities = ((imgL.cols / 8) + 15) & -16;
	//Bitweise Maskierung mit -16 beduetet dass die 4 letzten Bits zu 0 werden
	// => die Zahl ist dadurch automatisch eine Vielfache von 16
	calculatedNumDisparities *= 2;

	int minDisparity = -calculatedNumDisparities / 2;
	int numberOfDisparities = calculatedNumDisparities;

	//cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(0, 256, 5);
	sgbm->setPreFilterCap(63);
	sgbm->setUniquenessRatio(10);
	sgbm->setSpeckleWindowSize(100);
	sgbm->setSpeckleRange(2);
	sgbm->setDisp12MaxDiff(1);
	sgbm->setMode(cv::StereoSGBM::MODE_HH);
	sgbm->setP1(600);
	sgbm->setP2(2400);
	sgbm->compute(imgL, imgR, getResult());

	cv::Mat out;
	getResult().convertTo(out, CV_32F, 1.0 / 16);
	getResult() = out;

	double min, max;
	cv::minMaxLoc(getResult(), &min, &max);
	std::cout << "computedDisp min: " << min << " max: " << max << std::endl;

	/*// Save the image data in binary format
	std::ofstream os("/Users/bjohn/Desktop/test.exr",std::ios::out|std::ios::trunc|std::ios::binary);
	os << (int)out.rows << " " << (int)out.cols << " " << (int)out.type() << " ";
	os.write((char*)out.data,out.step.p[0]*out.rows);
	os.close();*/

	std::cout << "M = "<< std::endl << " "  << getResult().row(0) << std::endl << std::endl;

	//cv::minMaxLoc(getResult(), &min, &max);
	//std::cout << "computedDisp / 16 min: " << min << " max: " << max << std::endl;

	/*int unmatchedValue = -129;
	cv::Mat unmatchedMaskLR;
	unmatchedMaskLR = (getResult() == unmatchedValue);
	dilate(unmatchedMaskLR, unmatchedMaskLR, cv::Mat(), cv::Point(-1, -1), 2);

	cv::minMaxLoc(getNormalizedResult(), &min, &max);
	std::cout << "computedDisp normalized min: " << min << " max: " << max << std::endl;*/
}
