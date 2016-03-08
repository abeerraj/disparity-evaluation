#include "Bitmask.hpp"

Bitmask::Bitmask(int w, int h, bool state) {
  this->w = w;
  this->h = h;
  bitmask = boost::dynamic_bitset<>(w * h, false);
  if (state == true) {
    for (int y = 0; y < this->h; y++) {
      for (int x = 0; x < this->w; x++) {
        this->flip(x,y);
      }
    }
  }
}

int Bitmask::size() {
  return w * h;
}

void Bitmask::flip(int x, int y) {
  bitmask.flip(x + y * this->w);
}

bool Bitmask::get(int x, int y) {
  return bitmask.test(x + y * this->w);
}

cv::Mat Bitmask::mat() {
  cv::Mat mat = cv::Mat(this->h, this->w, CV_8UC1);
  for (int y = 0; y < this->h; y++) {
    for (int x = 0; x < this->w; x++) {
      mat.at<uchar>(y, x) = 0;
      if (this->get(x,y)) {
        mat.at<uchar>(y, x) = 255;
      }
    }
  }
  return mat;
}
