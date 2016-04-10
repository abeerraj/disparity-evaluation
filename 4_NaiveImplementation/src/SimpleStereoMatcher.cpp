#include "SimpleStereoMatcher.hpp"

using namespace std;
using namespace cv;

bool temporal;
bool weighted;
int maxDisparity = 64;
int windowSize = 9;
Mat dsi;

SimpleStereoMatcher::SimpleStereoMatcher(Configuration configuration, vector<string> images) {
	// apply configuration
	temporal = configuration.temporal;
	weighted = configuration.weighted;

	// create disparity space image (DSI)
	Mat meta = imread(configuration.left + "/" + images[0] + ".png", CV_LOAD_IMAGE_GRAYSCALE);
	const int size[4] = {meta.cols, meta.rows, (int) images.size(), maxDisparity};
	dsi = Mat(4, size, CV_32F);

	// fill DSI with all images iteratively
	for (int i = 0; i < images.size(); i++) {
		string image = images[i] + ".png";
		Mat left = imread(configuration.left + "/" + image, CV_LOAD_IMAGE_GRAYSCALE);
		Mat right = imread(configuration.right + "/" + image, CV_LOAD_IMAGE_GRAYSCALE);
		cout << "fill dsi for t = " << i << endl;
		fillDisparitySpaceImage(i, left, right);
	}
}

void SimpleStereoMatcher::fillDisparitySpaceImage(int t, Mat left, Mat right) {
	int step = (windowSize - 1) / 2;
	int width = left.cols;
	int height = left.rows;

	// iterate over every row in the left image
	for (int y = 0 + step; y < height - step; y++) {

		// iterate over every column in the left image
		for (int x = 0 + step + maxDisparity; x < width - step; x++) {

			// iterate over every disparity value in x-direction
			for (int d = 0; d <= maxDisparity; d++) {

				// matching window in the left image
				Mat matchingWindowLeft(left, Rect(x - step, y - step, windowSize, windowSize));

				// matching window in the right image
				Mat matchingWindowRight(right, Rect(x - d - step, y - step, windowSize, windowSize));

				// calculcate the matching cost (SAD)
				Mat matchingCostWindow;
				absdiff(matchingWindowLeft, matchingWindowRight, matchingCostWindow);
				double matchingCost = sum(matchingCostWindow)[0];

				// save matching cost in dsi for given t
				dsi.at<Vec4f>(x, y, t)[d] = (float) matchingCost;
			}
		}
	}
}

int SimpleStereoMatcher::minMatchingCostFrom(float matchingCostArray[]) {
	int disparities = dsi.size[3];

	double minvalue = 65536;
	int currentdisparity = -1;
	for (int d = 0; d < disparities; d++) {
		float matchingcost = matchingCostArray[d];
		if (matchingcost < minvalue) {
			minvalue = matchingcost;
			currentdisparity = d;
		}
	}

	return currentdisparity;
}

int SimpleStereoMatcher::findMinMatchingCost(int x, int y, int t) {
	int images = dsi.size[2];
	int disparities = dsi.size[3];

	int observedFrames = 3;
	float matchingCostArray[disparities];

	// no spatiotemporal approach possible
	if (images < observedFrames || !temporal) {
		for (int d = 0; d < disparities; d++) {
			float matchingcost = dsi.at<Vec4f>(x, y, t)[d];
			matchingCostArray[d] = matchingcost;
		}
		return minMatchingCostFrom(matchingCostArray);
	}

	// default normalized weight
	double fac[observedFrames];
	for (int i = 0; i < observedFrames; i++) {
		fac[i] = (double) 1 / observedFrames;
	}

	// custom weight
	if (weighted) {
		fac[0] = (double) 2 / 5;
		fac[1] = (double) 1 / 5;
		fac[2] = (double) 2 / 5;
	}

	bool first = (t == 0);
	bool last = (t == observedFrames - 1);

	for (int d = 0; d < disparities; d++) {
		float matchingcost[observedFrames];
		matchingcost[0] = dsi.at<Vec4f>(x, y, first ? 0 : t - 1)[d];
		matchingcost[1] = dsi.at<Vec4f>(x, y, t)[d];
		matchingcost[2] = dsi.at<Vec4f>(x, y, last ? observedFrames - 1 : t + 1)[d];

		matchingCostArray[d] = 0;
		for (int i = 0; i < observedFrames; i++) {
			matchingCostArray[d] += fac[i] * matchingcost[i];
		}
	}
	return minMatchingCostFrom(matchingCostArray);
}

Mat SimpleStereoMatcher::getDisparityMap(int t) {
	cout << "get dsi for t = " << t << endl;
	int width = dsi.size[0];
	int height = dsi.size[1];

	// final disparity map
	Mat dispMap(Size(width, height), CV_8UC1);

	// use the lowest matching cost, iterate again
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			dispMap.at<uchar>(y, x) = (uchar) findMinMatchingCost(x, y, t);
		}
	}

	return dispMap;
}
