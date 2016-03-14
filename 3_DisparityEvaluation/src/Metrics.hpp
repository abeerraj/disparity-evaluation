#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class Metrics
{
  public:
    static float getRMSE(const cv::Mat disparity, const cv::Mat groundTruth, const cv::Mat bitmask);
    static float getPercentageOfBadPixels(const cv::Mat calculatedDispartiyMat, const cv::Mat groundTruthMat, const cv::Mat bitmask, const float threshold);
};
