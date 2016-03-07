#include "OpenCVStereoSGBM.hpp"

void OpenCVStereoSGBM::compute() {
  // Ptr<StereoSGBM> sgbm = StereoSGBM::create(-64, 192, 5);
  Ptr<StereoSGBM> sgbm = StereoSGBM::create(0, 256 / 4, 5);
  sgbm->setPreFilterCap(4);
  sgbm->setUniquenessRatio(1);
  sgbm->setSpeckleWindowSize(150);
  sgbm->setSpeckleRange(2);
  sgbm->setDisp12MaxDiff(10);
  sgbm->setMode(StereoSGBM::MODE_HH);
  sgbm->setP1(600);
  sgbm->setP2(2400);
  sgbm->compute(imgL, imgR, getResult());
}
