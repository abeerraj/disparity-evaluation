#include "Bitmask.hpp"

Bitmask::Bitmask(cv::Mat mat) {
	binaryMat = mat;
}

Bitmask::Bitmask(int w, int h) {
	binaryMat = cv::Mat::zeros(cv::Size(w, h), CV_8UC1);
}

void Bitmask::flip(int y, int x) {
	bool state = get(y, x);
	if (state) {
		binaryMat.at<uchar>(y, x) = 0;
		return;
	}
	binaryMat.at<uchar>(y, x) = 255;
}

bool Bitmask::get(int y, int x) const {
	return binaryMat.at<uchar>(y, x) > 0;
}

const cv::Mat Bitmask::mat() const {
	return binaryMat;
}
