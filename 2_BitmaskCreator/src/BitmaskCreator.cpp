#include "BitmaskCreator.hpp"
#include "Utils.hpp"

Bitmask BitmaskCreator::getDepthDiscontinuedPixels(Frame frame, float gap, int width) {
  cv::Mat disp/* = frame.dispLeft*/;

  int cols = disp.cols;
  int rows = disp.rows;

  cv::Mat depthDiscontMask;
  cv::Mat curDisp;
  disp.copyTo(curDisp);
  cv::Mat maxNeighbDisp;
  dilate(curDisp, maxNeighbDisp, cv::Mat(3, 3, CV_8UC1, cv::Scalar(1)));
  cv::Mat minNeighbDisp;
  erode(curDisp, minNeighbDisp, cv::Mat(3, 3, CV_8UC1, cv::Scalar(1)));
  depthDiscontMask = max((cv::Mat)(maxNeighbDisp-disp), (cv::Mat)(disp-minNeighbDisp)) > gap;
  dilate(depthDiscontMask, depthDiscontMask, cv::Mat(width, width, CV_8UC1, cv::Scalar(1)));

  Bitmask mask = Bitmask(cols, rows);

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      uchar value = depthDiscontMask.at<uchar>(y, x);
      if (value > 0) {
        mask.flip(x, y);
      }
    }
  }
  return mask;
}

Bitmask BitmaskCreator::getTexturedPixels(Frame frame, int width, float threshold) {
  cv::Mat img = Utils::image(frame.path + frame.imgLeft);

  int cols = img.cols;
  int rows = img.rows;

  Bitmask mask = Bitmask(cols, rows);

  if (img.channels() > 1) {
    cv::Mat tmp;
    cvtColor(img, tmp, cv::COLOR_BGR2GRAY);
    img = tmp;
  }

  cv::Mat dxI;
  cv::Sobel(img, dxI, CV_32FC1, 1, 0, 3);
  cv::Mat dxI2;
  cv::pow(dxI / 8.0f/*normalize*/, 2, dxI2);
  cv::Mat avgDxI2;
  cv::boxFilter(dxI2, avgDxI2, CV_32FC1, cv::Size(width, width));

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      float value = avgDxI2.at<float>(y, x);
      if (value >= threshold) {
        mask.flip(x, y);
      }
    }
  }
  return mask;
}

Bitmask BitmaskCreator::getOccludedPixels(Frame frame, float threshold) {
  cv::Mat groundTruthL = Utils::image(frame.path + frame.truthLeft);
  cv::Mat groundTruthR = Utils::image(frame.path + frame.truthRight);

  int cols = groundTruthL.cols;
  int rows = groundTruthL.rows;

  Bitmask mask = Bitmask(cols, rows);

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      uchar leftVal = groundTruthL.at<uchar>(y, x);
      uchar rightVal = groundTruthR.at<uchar>(y, x);
      if (abs(leftVal - rightVal) < threshold) {
        mask.flip(x, y);
      }
    }
  }
  return mask;
}
