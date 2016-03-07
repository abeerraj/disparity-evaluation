#pragma once

#include "DisparityAlgorithm.hpp"

class MRFStereo : public DisparityAlgorithm {
  public:
    MRFStereo(std::string imgL, std::string imgR) {
      this->imgL = imgL;
      this->imgR = imgR;
    }

    void compute();

  private:
    std::string imgL, imgR;
};
