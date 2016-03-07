#pragma once

#include "DisparityAlgorithm.hpp"

class OpenCVStereoBM : public DisparityAlgorithm {
  public:
    OpenCVStereoBM(cv::Mat imgL, cv::Mat imgR) {
      this->imgL = imgL;
      this->imgR = imgR;
    }

    void compute();

  private:
    cv::Mat imgL, imgR;
};
