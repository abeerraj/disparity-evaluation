#include "Heatmap.hpp"

void Heatmap::generateHeatmap(const cv::Mat disp) {
	cv::Mat normalizedDisp;
	cv::normalize(disp, normalizedDisp, 0, 255, CV_MINMAX, CV_8UC1);

	cv::Mat heatmap;
	cv::applyColorMap(normalizedDisp, heatmap, cv::COLORMAP_AUTUMN);

	cv::imshow("heatmap", heatmap);
	cv::waitKey(0);
}
