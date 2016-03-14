#pragma once

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

class Utils {
public:
	static const cv::Mat depth2disparity(const cv::Mat depth,
	                                     double baseline_separation,
	                                     double zero_disp_dist,
	                                     double render_width,
	                                     double focal_length,
	                                     double sensor_width = 32);
};
