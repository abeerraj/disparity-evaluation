#include "OpenCVStereoBM.hpp"

void OpenCVStereoBM::compute() {
	cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create(128, 9);
	sbm->setPreFilterSize(5);
	sbm->setPreFilterCap(61);
	sbm->setMinDisparity(-39);
	sbm->setTextureThreshold(507);
	sbm->setUniquenessRatio(0);
	sbm->setSpeckleWindowSize(0);
	sbm->setSpeckleRange(8);
	sbm->setDisp12MaxDiff(1);
	sbm->compute(imgL, imgR, this->result);
}
