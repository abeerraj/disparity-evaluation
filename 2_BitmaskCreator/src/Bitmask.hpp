#pragma once

#include <boost/dynamic_bitset.hpp>
#include <opencv2/opencv.hpp>

class Bitmask {
  public:
    Bitmask() {}

    // all bits should be false initially
    Bitmask(int w, int h, bool state = false);

    int size();
    void flip(int x, int y);
    bool get(int x, int y);
    cv::Mat mat();

  private:
    boost::dynamic_bitset<> bitmask;
    int w;
    int h;
};
