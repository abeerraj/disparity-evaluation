#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class Metrics {
public:
	static double getRMSE(cv::Mat disparity, cv::Mat groundTruth, cv::Mat bitmask);

	static double getPercentageOfBadPixels(cv::Mat disp, cv::Mat groundTruth, cv::Mat bitmask, float threshold);
};
