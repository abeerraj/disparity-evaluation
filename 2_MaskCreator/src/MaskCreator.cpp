#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/saliency.hpp>
#include "MaskCreator.hpp"

using namespace std;
using namespace cv;

const Mat MaskCreator::getOccludedPixels(Mat dispTruthLeft, Mat dispTruthRight, float threshold) {
	int cols = dispTruthLeft.cols;
	int rows = dispTruthLeft.rows;

	Mat mask = Mat::zeros(Size(cols, rows), CV_8UC1);
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			float leftVal = dispTruthLeft.at<float>(y, x);
			float rightVal = dispTruthRight.at<float>(y, x);
			if (abs(leftVal - rightVal) < threshold) {
				mask.at<uchar>(y, x) = 255;
			}
		}
	}
	return mask;
}

const Mat MaskCreator::getTexturedPixels(Mat left, int width, float threshold) {
	int cols = left.cols;
	int rows = left.rows;

	Mat mask = Mat::zeros(Size(cols, rows), CV_8UC1);

	if (left.channels() > 1) {
		Mat tmp;
		cvtColor(left, tmp, COLOR_BGR2GRAY);
		left = tmp;
	}

	Mat dxI;
	Sobel(left, dxI, CV_32FC1, 1, 0, 3);
	Mat dxI2;
	pow(dxI / 8.0f/*normalize*/, 2, dxI2);
	Mat avgDxI2;
	boxFilter(dxI2, avgDxI2, CV_32FC1, Size(width, width));

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			float value = avgDxI2.at<float>(y, x);
			if (value >= threshold) {
				mask.at<uchar>(y, x) = 255;
			}
		}
	}
	return mask;
}

const Mat MaskCreator::getDepthDiscontinuedPixels(Mat dispTruthLeft, float gap, int width) {
	int cols = dispTruthLeft.cols;
	int rows = dispTruthLeft.rows;

	Mat depthDiscontMask;
	Mat curDisp;
	dispTruthLeft.copyTo(curDisp);
	Mat maxNeighbDisp;
	dilate(curDisp, maxNeighbDisp, Mat(3, 3, CV_8UC1, Scalar(1)));
	Mat minNeighbDisp;
	erode(curDisp, minNeighbDisp, Mat(3, 3, CV_8UC1, Scalar(1)));
	depthDiscontMask = max((Mat) (maxNeighbDisp - dispTruthLeft), (Mat) (dispTruthLeft - minNeighbDisp)) > gap;
	dilate(depthDiscontMask, depthDiscontMask, Mat(width, width, CV_8UC1, Scalar(1)));

	Mat mask = Mat::zeros(Size(cols, rows), CV_8UC1);

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			uchar value = depthDiscontMask.at<uchar>(y, x);
			if (value > 0) {
				mask.at<uchar>(y, x) = 255;
			}
		}
	}
	return mask;
}

const Mat MaskCreator::getSalientPixels(Mat left) {
	Ptr<saliency::Saliency> saliencyAlgorithm = saliency::Saliency::create("SPECTRAL_RESIDUAL");
	Mat saliencyMap, binaryMap;
	if (saliencyAlgorithm->computeSaliency(left, saliencyMap)) {
		saliency::StaticSaliencySpectralResidual spec;
		spec.computeBinaryMap(saliencyMap, binaryMap);
	}
	return binaryMap;
}
