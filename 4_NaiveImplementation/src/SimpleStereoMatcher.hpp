#pragma once

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class SimpleStereoMatcher {
public:
	SimpleStereoMatcher();

	cv::Mat createDisparitySpaceImage(cv::Mat left, cv::Mat right, int windowSize, int minDisparity, int maxDisparity);

	cv::Mat getDisparityMap(cv::Mat dsi);
};
