#pragma once

#include <iostream>
#include <opencv2/core.hpp>

class Frame {
  public:
    int id;
    std::string path;
    std::string base;
    std::string imgLeft;
    std::string imgRight;
    std::string truthLeft;
    std::string truthRight;

    friend std::ostream& operator<<(std::ostream& os, const Frame& frame) {
      return os
      << "Frame(" << std::endl
      << "\t" << "id=" << frame.id << std::endl
      << "\t" << "base=" << frame.base << std::endl
      << "\t" << "imgLeft=" << frame.imgLeft << std::endl
      << "\t" << "imgRight=" << frame.imgRight << std::endl
      << "\t" << "truthLeft=" << frame.truthLeft << std::endl
      << "\t" << "truthRight=" << frame.truthRight << std::endl
      << ")";
    }
};
