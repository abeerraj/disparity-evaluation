#pragma once

#include "DisparityAlgorithm.hpp"

class OpenCVStereoBM : public DisparityAlgorithm {
  public:
    OpenCVStereoBM(Mat imgL, Mat imgR) {
      this->imgL = imgL;
      this->imgR = imgR;
    }

    void compute();

  private:
    Mat imgL, imgR;
};
