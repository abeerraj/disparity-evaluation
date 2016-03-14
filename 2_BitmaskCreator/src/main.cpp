#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <common/Constants.hpp>
#include <opencv2/highgui.hpp>
#include "BitmaskCreator.hpp"
#include "Configuration.hpp"

const Configuration parseCommandLineArguments(const char *argv[]) {
	std::string left = argv[1];
	std::string dispTruthLeft = argv[2];
	std::string dispTruthRight = argv[3];
	return Configuration(left, dispTruthLeft, dispTruthRight);
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		std::cout << "Usage: " << argv[0] << " <left> <dispTruthLeft> <dispTruthRight>" << std::endl;
		exit(1);
	}

	const Configuration configuration = parseCommandLineArguments(argv);
	cv::Mat left = cv::imread(Constants::workDir + configuration.left);
	cv::Mat dispTruthLeft = cv::imread(Constants::workDir + configuration.dispTruthLeft);
	cv::Mat dispTruthRight = cv::imread(Constants::workDir + configuration.dispTruthRight);

	const cv::Mat texturedMask = BitmaskCreator::getTexturedPixels(left);
	const cv::Mat occludedMask = BitmaskCreator::getOccludedPixels(dispTruthLeft, dispTruthRight);
	const cv::Mat depthDiscontinuityMask = BitmaskCreator::getDepthDiscontinuedPixels(dispTruthLeft);
	const cv::Mat salientMask = BitmaskCreator::getSalientPixels(left);
	cv::imshow("texturedMask", texturedMask);
	cv::imshow("occludedMask", occludedMask);
	cv::imshow("depthDiscontinuityMask", depthDiscontinuityMask);
	cv::waitKey(0);

	// TODO write output

	return 0;
}
