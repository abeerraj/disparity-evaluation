#pragma once

#include "DisparityAlgorithm.hpp"

class OpenCVStereoSGBM : public DisparityAlgorithm {
  public:
    OpenCVStereoSGBM(Mat imgL, Mat imgR) {
      this->imgL = imgL;
      this->imgR = imgR;
    }

    void compute();

  private:
    Mat imgL, imgR;
};
