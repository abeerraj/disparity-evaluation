#include "Metrics.hpp"

float Metrics::getRMSE(const cv::Mat disparity, const cv::Mat groundTruth, const cv::Mat bitmask)
{
  int cols = disparity.cols;
  int rows = disparity.rows;
  int numPixels = cols * rows;

  double rmsError = 0.0;

  for (int y = 0; y < rows; y++)
  {
    for (int x = 0; x < cols; x++)
    {
      // TODO layered bitmasks
      if (!bitmask.get(x, y)) continue;
      uchar actual = disparity.at<uchar>(y, x) / 4;
      uchar expected = groundTruth.at<uchar>(y, x) / 4;
      // TODO skip black pixels for now (unknown disparity)
      if (actual == 0) continue;
      rmsError += pow(abs(actual - expected), 2);
    }
  }

  return sqrt(rmsError / numPixels);
}

/*
* To obtain disparity (in pixels) from pixel values, devide the pixel value by 4.
* This maps pixel values 0 to 255 linearly to disparities 0 to 63.75, in steps of 0.25.
*/
float Metrics::getPercentageOfBadPixels(const cv::Mat disparity, const cv::Mat groundTruth, const cv::Mat bitmask, float threshold)
{
  int numBadPixels = 0;
  int numTotalPixels = 0;
  int cols = disparity.cols;
  int rows = disparity.rows;

  for (int y = 0; y < rows; y++)
  {
    for (int x = 0; x < cols; x++)
    {
      // TODO layered bitmasks
      if (!bitmask.get(x, y)) continue;
      uchar actual = disparity.at<uchar>(y, x) / 4;
      uchar expected = groundTruth.at<uchar>(y, x) / 4;
      // TODO skip black pixels for now (unknown disparity)
      if (actual == 0) continue;
      if (abs(actual - expected) > threshold)
      numBadPixels++;
      numTotalPixels++;
    }
  }

  return 100.0f * (float) numBadPixels / (float) numTotalPixels;
}
