#include <iostream>
#include "Utils.hpp"
#include "Metrics.hpp"

const int unknownDisparity = 0;

double Metrics::getRMSE(const cv::Mat disparity,
                       const cv::Mat groundTruth,
                       const cv::Mat bitmask) {
	int cols = disparity.cols;
	int rows = disparity.rows;

	int numPixels = 0;
	float rmsError = 0.0;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			if (!Utils::isSet(bitmask, y, x)) continue;
			float actual = disparity.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			if (actual == unknownDisparity) continue;
			numPixels++;
			rmsError += pow(fabsf(actual - expected), 2);
		}
	}

	return sqrt(rmsError / numPixels);
}

double Metrics::getPercentageOfBadPixels(const cv::Mat disparity,
                                        const cv::Mat groundTruth,
                                        const cv::Mat bitmask,
                                        float threshold) {
	int numBadPixels = 0;
	int numTotalPixels = 0;
	int cols = disparity.cols;
	int rows = disparity.rows;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			if (!Utils::isSet(bitmask, y, x)) continue;
			float actual = disparity.at<float>(y, x);
			float expected = groundTruth.at<float>(y, x);
			if (actual == unknownDisparity) continue;
			if (fabsf(actual - expected) > threshold)
				numBadPixels++;
			numTotalPixels++;
		}
	}
	std::cout << "numBadPixels: " << numBadPixels << std::endl;
	std::cout << "numTotalPixels: " << numTotalPixels << std::endl;
	std::cout << "numMatPixels: " << cols * rows << std::endl << std::endl;

	return 100.0 * numBadPixels / numTotalPixels;
}
