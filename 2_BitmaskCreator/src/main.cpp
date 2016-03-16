#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <common/Constants.hpp>
#include <opencv2/highgui.hpp>
#include "BitmaskCreator.hpp"
#include "Configuration.hpp"

const Configuration parseCommandLineArguments(const char *argv[]) {
	std::string left = argv[1];
	std::string dispLeft = argv[2];
	std::string dispTruthLeft = argv[3];
	std::string dispTruthRight = argv[4];
	return Configuration(left, dispLeft, dispTruthLeft, dispTruthRight);
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		std::cout << "Usage: " << argv[0] << " <left> <dispLeft> <dispTruthLeft> <dispTruthRight>" << std::endl;
		exit(1);
	}

	const Configuration configuration = parseCommandLineArguments(argv);
	cv::Mat left = cv::imread(Constants::workDir + configuration.left);
	cv::Mat dispLeft = cv::imread(Constants::workDir + configuration.dispLeft, CV_LOAD_IMAGE_ANYDEPTH);
	cv::Mat dispTruthLeft = cv::imread(Constants::workDir + configuration.dispTruthLeft, CV_LOAD_IMAGE_ANYDEPTH);
	cv::Mat dispTruthRight = cv::imread(Constants::workDir + configuration.dispTruthRight, CV_LOAD_IMAGE_ANYDEPTH);

	const cv::Mat texturedMask = BitmaskCreator::getTexturedPixels(left);
	const cv::Mat occludedMask = BitmaskCreator::getOccludedPixels(dispTruthLeft, dispTruthRight);
	const cv::Mat depthDiscontinuityMask = BitmaskCreator::getDepthDiscontinuedPixels(dispTruthLeft);
	const cv::Mat salientMask = BitmaskCreator::getSalientPixels(left);
	const cv::Mat unknownMask = BitmaskCreator::getUnknownDisparityPixels(dispLeft);
	cv::imshow("unknownMask", unknownMask);
	cv::imshow("texturedMask", texturedMask);
	cv::imshow("occludedMask", occludedMask);
	cv::imshow("salientMask", salientMask);
	cv::imshow("depthDiscontinuityMask", depthDiscontinuityMask);
	cv::waitKey(0);

	// TODO save bitmasks
	cv::imwrite(Constants::workDir + "bitmask-textured.png", texturedMask);
	cv::imwrite(Constants::workDir + "bitmask-occluded.png", occludedMask);
	cv::imwrite(Constants::workDir + "bitmask-unknown.png", unknownMask);
	cv::imwrite(Constants::workDir + "bitmask-salient.png", salientMask);
	cv::imwrite(Constants::workDir + "bitmask-depthDiscontinuityMask.png", depthDiscontinuityMask);

	return 0;
}
