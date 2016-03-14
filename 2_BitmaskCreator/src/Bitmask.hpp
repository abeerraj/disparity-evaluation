#pragma once

#include <opencv2/core.hpp>

class Bitmask {
public:
	Bitmask(cv::Mat mat);

	Bitmask(int width, int height);

	void flip(int x, int y);

	bool get(int x, int y) const;

	const cv::Mat mat() const;

private:
	cv::Mat binaryMat;
};
