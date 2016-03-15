#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class Metrics {
public:
	static float getRMSE(const cv::Mat disparity,
	                     const cv::Mat groundTruth,
	                     const cv::Mat bitmask);

	static float getPercentageOfBadPixels(const cv::Mat disparity,
	                                      const cv::Mat groundTruth,
	                                      const cv::Mat bitmask,
	                                      float threshold = 4.0f);

private:
	static bool isSet(const cv::Mat bitmask, int y, int x);
};
