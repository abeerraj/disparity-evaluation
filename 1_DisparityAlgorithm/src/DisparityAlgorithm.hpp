#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class DisparityAlgorithm {
public:
	virtual void compute() = 0;

	virtual ~DisparityAlgorithm() { };

	cv::Mat &getResult() {
		return result;
	}

	void setResult(cv::Mat result) {
		this->result = result;
	}

	cv::Mat getNormalizedResult() {
		cv::Mat normalized;
		normalize(result, normalized, 0, 255, CV_MINMAX, CV_8U);
		return normalized;
	}

private:
	cv::Mat result;
};
