#include <iostream>
#include "Metrics.hpp"

bool Metrics::isSet(const cv::Mat bitmask, int y, int x) {
	uchar val = bitmask.at<uchar>(y, x);
	return val == 255;
}

float Metrics::getRMSE(const cv::Mat disparity,
                       const cv::Mat groundTruth,
                       const cv::Mat bitmask) {
	int cols = disparity.cols;
	int rows = disparity.rows;
	int numPixels = cols * rows;

	double rmsError = 0.0;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			// TODO layered bitmasks
			if (Metrics::isSet(bitmask, y, x)) continue;
			float actual = disparity.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			rmsError += pow(std::abs(actual - expected), 2);
		}
	}

	return sqrt(rmsError / numPixels);
}

float Metrics::getPercentageOfBadPixels(const cv::Mat disparity,
                                        const cv::Mat groundTruth,
                                        const cv::Mat bitmask,
                                        float threshold) {
	int numBadPixels = 0;
	int numTotalPixels = 0;
	int cols = disparity.cols;
	int rows = disparity.rows;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			// TODO layered bitmasks
			if (Metrics::isSet(bitmask, y, x)) continue;
			float actual = disparity.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			if (std::abs(actual - expected) > threshold)
				numBadPixels++;
			numTotalPixels++;
		}
	}
	std::cout << "numBadPixels: " << numBadPixels << std::endl;
	std::cout << "numTotalPixels: " << numTotalPixels << std::endl << std::endl;

	return 100.0f * (float) numBadPixels / (float) numTotalPixels;
}
