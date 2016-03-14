#pragma once

#include <opencv2/core/core.hpp>

class BitmaskCreator {
public:
	static const cv::Mat getOccludedPixels(cv::Mat dispTruthLeft, cv::Mat dispTruthRight, float threshold = 1.0f);

	static const cv::Mat getTexturedPixels(cv::Mat left, int width = 3, float threshold = 4.0f);

	static const cv::Mat getDepthDiscontinuedPixels(cv::Mat dispTruthLeft, float gap = 2.0f, int width = 9);

	static const cv::Mat getSalientPixels(cv::Mat left);
};
