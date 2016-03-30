#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "StereoMatcher.hpp"

using namespace std;
using namespace cv;

Mat createDisparitySpaceImage(Mat left, Mat right, int windowSize, int minDisparity, int maxDisparity) {
	int dMin = minDisparity;
	int dMax = maxDisparity;
	int step = (windowSize - 1) / 2;

	int width = left.cols;
	int height = left.rows;

	// create disparity space image (DSI)
	const int size[3] = {width, height, dMax - dMin};
	Mat dsi(3, size, CV_32F);

	// iterate over every row in the left image
	for (int y = 0 + step; y < height - step; y++) {

		// iterate over every column in the left image
		for (int x = 0 + step; x < width - step - dMax; x++) {
			
			// iterate over every disparity value in x-direction
			for (int d = dMin; d <= dMax; d++) {

				// matching window in the left image
				Mat matchingWindowLeft(left, Rect(x - step, y - step, windowSize, windowSize));

				// matching window in the right image
				Mat matchingWindowRight(right, Rect(x + d - step, y - step, windowSize, windowSize));

				// calculcate the matching cost (SAD)
				Mat matchingCostWindow;
				absdiff(matchingWindowLeft, matchingWindowRight, matchingCostWindow);
				double matchingCost = sum(matchingCostWindow)[0];

				// save matching cost in dsi
				dsi.at<float>(x, y, d) = (float) matchingCost;
			}
		}
	}
	return dsi;
}

Mat getDisparityMap(Mat dsi) {
	int width = dsi.size[0];
	int height = dsi.size[1];
	int disparities = dsi.size[2];

	// final disparity map
	Mat dispMap(Size(width, height), CV_8UC1);

	// use the lowest matching cost, iterate again
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			double minvalue = 65536;
			int currentdisparity = -1;
			for (int d = 0; d < disparities; d++) {
				float matchingcost = dsi.at<float>(x, y, d);
				if (matchingcost < minvalue) {
					minvalue = matchingcost;
					currentdisparity = d;
				}
			}
			dispMap.at<uchar>(y, x) = (uchar) currentdisparity;
		}
	}

	return dispMap;
}

int main(int argc, const char *argv[]) {
	Mat left = imread("/Users/bjohn/Desktop/datasets/tsukuba/01_tsukuba/right/image0001.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat right = imread("/Users/bjohn/Desktop/datasets/tsukuba/01_tsukuba/left/image0001.png", CV_LOAD_IMAGE_GRAYSCALE);

	int dMin = 0;
	int dMax = 16;
	int windowSize = 9;

	Mat dsi = createDisparitySpaceImage(left, right, windowSize, dMin, dMax);
	Mat dispMap = getDisparityMap(dsi);

	// show computed disparity map
	Mat adjMap;
	float scale = 255 / (dMax - dMin);
	dispMap.convertTo(adjMap, CV_8UC1, scale, -dMin * scale);
	Mat heatmap;
	applyColorMap(adjMap, heatmap, COLORMAP_JET);
	imshow("heatmap", heatmap);
	waitKey(0);

	return 0;
}
