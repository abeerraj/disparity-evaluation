#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/saliency.hpp>
#include "BitmaskCreator.hpp"
#include "Bitmask.hpp"

const cv::Mat BitmaskCreator::getOccludedPixels(cv::Mat dispTruthLeft, cv::Mat dispTruthRight, float threshold) {
	int cols = dispTruthLeft.cols;
	int rows = dispTruthLeft.rows;

	Bitmask mask = Bitmask(cols, rows);
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			uchar leftVal = dispTruthLeft.at<uchar>(y, x);
			uchar rightVal = dispTruthRight.at<uchar>(y, x);
			if (abs(leftVal - rightVal) < threshold) {
				mask.flip(y, x);
			}
		}
	}
	return mask.mat();
}

const cv::Mat BitmaskCreator::getTexturedPixels(cv::Mat left, int width, float threshold) {
	int cols = left.cols;
	int rows = left.rows;

	Bitmask mask = Bitmask(cols, rows);

	if (left.channels() > 1) {
		cv::Mat tmp;
		cvtColor(left, tmp, cv::COLOR_BGR2GRAY);
		left = tmp;
	}

	cv::Mat dxI;
	cv::Sobel(left, dxI, CV_32FC1, 1, 0, 3);
	cv::Mat dxI2;
	cv::pow(dxI / 8.0f/*normalize*/, 2, dxI2);
	cv::Mat avgDxI2;
	cv::boxFilter(dxI2, avgDxI2, CV_32FC1, cv::Size(width, width));

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			float value = avgDxI2.at<float>(y, x);
			if (value >= threshold) {
				mask.flip(y, x);
			}
		}
	}
	return mask.mat();
}

const cv::Mat BitmaskCreator::getDepthDiscontinuedPixels(cv::Mat dispTruthLeft, float gap, int width) {
	int cols = dispTruthLeft.cols;
	int rows = dispTruthLeft.rows;

	cv::Mat depthDiscontMask;
	cv::Mat curDisp;
	dispTruthLeft.copyTo(curDisp);
	cv::Mat maxNeighbDisp;
	dilate(curDisp, maxNeighbDisp, cv::Mat(3, 3, CV_8UC1, cv::Scalar(1)));
	cv::Mat minNeighbDisp;
	erode(curDisp, minNeighbDisp, cv::Mat(3, 3, CV_8UC1, cv::Scalar(1)));
	depthDiscontMask = max((cv::Mat) (maxNeighbDisp - dispTruthLeft), (cv::Mat) (dispTruthLeft - minNeighbDisp)) > gap;
	dilate(depthDiscontMask, depthDiscontMask, cv::Mat(width, width, CV_8UC1, cv::Scalar(1)));

	Bitmask mask = Bitmask(cols, rows);

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			uchar value = depthDiscontMask.at<uchar>(y, x);
			if (value > 0) {
				mask.flip(y, x);
			}
		}
	}
	return mask.mat();
}

const cv::Mat BitmaskCreator::getSalientPixels(cv::Mat left) {
	cv::Ptr<cv::saliency::Saliency> saliencyAlgorithm = cv::saliency::Saliency::create("SPECTRAL_RESIDUAL");
	cv::Mat saliencyMap, binaryMap;
	if (saliencyAlgorithm->computeSaliency(left, saliencyMap)) {
		cv::saliency::StaticSaliencySpectralResidual spec;
		spec.computeBinaryMap(saliencyMap, binaryMap);
	}
	return binaryMap;
}
