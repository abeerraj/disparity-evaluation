#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Configuration.hpp"

class SimpleStereoMatcher {
public:
	SimpleStereoMatcher(Configuration configuration, std::vector<std::string> images);

	cv::Mat getDisparityMap(int t);

private:
	void fillDisparitySpaceImage(int t, cv::Mat left, cv::Mat right);

	int findMinMatchingCost(int x, int y, int t);

	int minMatchingCostFrom(float matchingCostArray[]);
};
