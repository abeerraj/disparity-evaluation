#pragma once

#include <opencv2/core/core.hpp>

class MaskCreator {
public:
	static const cv::Mat getOccludedPixels(cv::Mat dispTruthLeft, cv::Mat dispTruthRight, float threshold = 1.0f);

	static const cv::Mat getTexturedPixels(cv::Mat left, int width = 2, float threshold = 16.0f);

	static const cv::Mat getDepthDiscontinuedPixels(cv::Mat dispTruthLeft, float gap = 2.0f, int width = 5);

	static const cv::Mat getSalientPixels(cv::Mat left);
};
