#include "Heatmap.hpp"

cv::Mat Heatmap::generateHeatmap(const cv::Mat disp) {
	cv::Mat normalizedDisp;
	cv::normalize(disp, normalizedDisp, 0, 255, CV_MINMAX, CV_8UC1);

	cv::Mat heatmap;
	cv::applyColorMap(normalizedDisp, heatmap, cv::COLORMAP_AUTUMN);

	return heatmap;
}

cv::Mat Heatmap::generateHeatmap(const cv::Mat disp, double min, double max) {
	cv::Mat adjMap;
	disp.convertTo(adjMap, CV_8UC1, 255 / (max - min), -min);

	cv::Mat heatmap;
	applyColorMap(adjMap, heatmap, cv::COLORMAP_AUTUMN);

	return heatmap;
}
