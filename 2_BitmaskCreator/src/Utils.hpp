#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Frame.hpp"

class Utils {
  public:
    static std::vector<Frame> getAllFramesFromPath(std::string path);
    static cv::Mat image(std::string filename);
};
