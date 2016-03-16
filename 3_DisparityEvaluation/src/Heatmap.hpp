#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class Heatmap {
public:
	static cv::Mat generateHeatmap(const cv::Mat disp);

	static cv::Mat generateHeatmap(const cv::Mat disp, double min, double max);
};