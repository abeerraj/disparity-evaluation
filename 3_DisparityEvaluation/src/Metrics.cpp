#include <iostream>
#include "Metrics.hpp"

using namespace std;
using namespace cv;

const int unknownDisparity = 0;

double Metrics::getRMSE(const Mat disparity, const Mat groundTruth, const Mat bitmask) {
	int cols = disparity.cols;
	int rows = disparity.rows;

	int numPixels = 0;
	float rmsError = 0.0;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			if (bitmask.at<uchar>(y, x) == 0) continue;
			float actual = disparity.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			if (actual == unknownDisparity) continue;
			numPixels++;
			rmsError += pow(fabsf(actual - expected), 2);
		}
	}

	return sqrt(rmsError / numPixels);
}

double Metrics::getPercentageOfBadPixels(const Mat disp, const Mat groundTruth, const Mat bitmask, float threshold) {
	int numBadPixels = 0;
	int numTotalPixels = 0;
	int cols = disp.cols;
	int rows = disp.rows;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			if (bitmask.at<uchar>(y, x) == 0) continue;
			float actual = disp.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			if (actual == unknownDisparity) continue;
			if (fabsf(actual - expected) > threshold)
				numBadPixels++;
			numTotalPixels++;
		}
	}
	cout << "numBadPixels: " << numBadPixels << endl;
	cout << "numTotalPixels: " << numTotalPixels << endl;
	cout << "numMatPixels: " << cols * rows << endl << endl;

	return 100.0 * numBadPixels / numTotalPixels;
}
