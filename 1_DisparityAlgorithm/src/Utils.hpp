#pragma once

#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"

class Utils {
public:
	static cv::Mat depth2disparity(const cv::Mat depth, double baseline_separation, double zero_disp_dist,
	                                      double render_width, double focal_length, double sensor_width = 32);
};
