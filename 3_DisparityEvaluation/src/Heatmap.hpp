#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class Heatmap {
public:
	static cv::Mat generateHeatmap(cv::Mat disp, double min, double max, int colormap = cv::COLORMAP_AUTUMN);

	static cv::Mat generateHeatmap(cv::Mat disp, double min, double max, cv::Mat unknown);

	static cv::Mat generateOutliersHeatmap(cv::Mat disp, cv::Mat truth, cv::Mat mask, cv::Mat occ, double min, double max);
};
