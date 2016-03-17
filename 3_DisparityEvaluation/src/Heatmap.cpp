#include "Heatmap.hpp"
#include "Utils.hpp"

cv::Mat Heatmap::generateHeatmap(const cv::Mat disp) {
	cv::Mat normalizedDisp;
	cv::normalize(disp, normalizedDisp, 0, 255, CV_MINMAX, CV_8UC1);

	cv::Mat heatmap;
	cv::applyColorMap(normalizedDisp, heatmap, cv::COLORMAP_AUTUMN);

	return heatmap;
}

cv::Mat Heatmap::generateHeatmap(const cv::Mat disp, double min, double max, int colormap) {
	cv::Mat adjMap;
	disp.convertTo(adjMap, CV_8UC1, 255 / (max - min), -min);

	cv::Mat heatmap;
	applyColorMap(adjMap, heatmap, colormap);

	return heatmap;
}

struct RGB {
	uchar blue;
	uchar green;
	uchar red;
};

cv::Mat Heatmap::generateHeatmap(const cv::Mat disp, double min, double max, const cv::Mat bitmask) {
	cv::Mat heatmap = generateHeatmap(disp, min, max);

	for (int y = 0; y < disp.rows; y++) {
		for (int x = 0; x < disp.cols; x++) {
			if (!Utils::isSet(bitmask, y, x)) {
				RGB &rgb = heatmap.ptr<RGB>(y)[x];
				rgb.red = 138;
				rgb.green = 36;
				rgb.blue = 255;
			}
		}
	}
	return heatmap;
}
