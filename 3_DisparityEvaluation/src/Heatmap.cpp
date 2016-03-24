#include "Heatmap.hpp"
#include "Utils.hpp"

struct RGB {
	uchar blue;
	uchar green;
	uchar red;
};

cv::Mat Heatmap::generateHeatmap(const cv::Mat disp, double min, double max, int colormap) {
	cv::Mat adjMap;
	float scale = 255 / (max - min);
	disp.convertTo(adjMap, CV_8UC1, scale, -min * scale);

	cv::Mat heatmap;
	applyColorMap(adjMap, heatmap, colormap);

	// applying hsv colouring to values
	/*cv::Mat normalizedDisp, hsvHeatmap;
	cv::normalize(heatmap, normalizedDisp, 0, 180, CV_MINMAX, CV_8UC3);
	cv::cvtColor(normalizedDisp, hsvHeatmap, CV_HSV2BGR);
	return hsvHeatmap;*/
	
	return heatmap;
}

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

cv::Mat Heatmap::generateOutliersHeatmap(const cv::Mat disparity, const cv::Mat groundTruth, double min, double max,
                                         float threshold) {
	cv::Mat heatmap = generateHeatmap(disparity, min, max);

	int cols = disparity.cols;
	int rows = disparity.rows;

	// mark outliers
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			float actual = disparity.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			if (fabsf(actual - expected) > threshold) {
				RGB &rgb = heatmap.ptr<RGB>(y)[x];
				rgb.red = 138;
				rgb.green = 36;
				rgb.blue = 255;
			}
		}
	}

	return heatmap;
}
