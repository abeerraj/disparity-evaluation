#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class Metrics {
public:
	static double getRMSE(const cv::Mat disparity,
	                     const cv::Mat groundTruth,
	                     const cv::Mat bitmask);

	static double getPercentageOfBadPixels(const cv::Mat disparity,
	                                      const cv::Mat groundTruth,
	                                      const cv::Mat bitmask,
	                                      float threshold = 4.0f);
};
