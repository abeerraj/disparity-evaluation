#include "SimpleStereoMatcher.hpp"

using namespace std;
using namespace cv;

SimpleStereoMatcher::SimpleStereoMatcher() {

}

Mat SimpleStereoMatcher::createDisparitySpaceImage(Mat left, Mat right, int windowSize, int minDisparity, int maxDisparity) {
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

Mat SimpleStereoMatcher::getDisparityMap(Mat dsi) {
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
