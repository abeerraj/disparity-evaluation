#include "OpenCVStereoBM.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"

using namespace cv;

void OpenCVStereoBM::compute() {
  Ptr<StereoBM> sbm = StereoBM::create(128, 9);
  sbm->setPreFilterSize(5);
  sbm->setPreFilterCap(61);
  sbm->setMinDisparity(-39);
  sbm->setTextureThreshold(507);
  sbm->setUniquenessRatio(0);
  sbm->setSpeckleWindowSize(0);
  sbm->setSpeckleRange(8);
  sbm->setDisp12MaxDiff(1);
  sbm->compute(imgL, imgR, getResult());
}
