#include "Heatmap.hpp"

using namespace std;
using namespace cv;

struct RGB {
	uchar blue;
	uchar green;
	uchar red;
};

Mat Heatmap::generateHeatmap(Mat disp, double min, double max, int colormap) {
	Mat adjMap;
	double scale = 255 / (max - min);
	disp.convertTo(adjMap, CV_8UC1, scale, -min * scale);

	Mat heatmap;
	applyColorMap(adjMap, heatmap, colormap);

	// applying hsv colouring to values
	/*Mat normalizedDisp, hsvHeatmap;
	normalize(heatmap, normalizedDisp, 0, 180, CV_MINMAX, CV_8UC3);
	cvtColor(normalizedDisp, hsvHeatmap, CV_HSV2BGR);
	return hsvHeatmap;*/

	return heatmap;
}

Mat Heatmap::generateHeatmap(Mat disp, double min, double max, const Mat bitmask) {
	Mat heatmap = generateHeatmap(disp, min, max);

	for (int y = 0; y < disp.rows; y++) {
		for (int x = 0; x < disp.cols; x++) {
			if (bitmask.at<uchar>(y, x) == 255) continue;
			RGB &rgb = heatmap.ptr<RGB>(y)[x];
			rgb.red = 0;
			rgb.green = 0;
			rgb.blue = 0;
		}
	}
	return heatmap;
}

Mat Heatmap::generateOutliersHeatmap(Mat disparity, Mat groundTruth, Mat bitmask, double min, double max,
                                     float threshold) {
	Mat heatmap = generateHeatmap(disparity, min, max);

	int cols = disparity.cols;
	int rows = disparity.rows;

	// mark outliers
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			float actual = disparity.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			if (bitmask.at<uchar>(y, x) == 0) {
				RGB &rgb = heatmap.ptr<RGB>(y)[x];
				rgb.red = 0;
				rgb.green = 0;
				rgb.blue = 0;
				continue;
			}
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
