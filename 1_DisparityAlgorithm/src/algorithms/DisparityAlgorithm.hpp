#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

class DisparityAlgorithm {
public:
	virtual void compute() = 0;

	virtual ~DisparityAlgorithm() { };

	const cv::Mat &getResult() const {
		return result;
	}

protected:
	cv::Mat result;
};
