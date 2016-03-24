#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <common/Constants.hpp>
#include <opencv2/highgui.hpp>
#include "BitmaskCreator.hpp"
#include "Configuration.hpp"

using namespace std;
using namespace cv;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	string left = argv[1];
	string dispLeft = argv[2];
	string dispTruthLeft = argv[3];
	string dispTruthRight = argv[4];
	configuration = {left, dispLeft, dispTruthLeft, dispTruthRight};
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		cout << "Usage: " << argv[0] << " <left> <dispLeft> <dispTruthLeft> <dispTruthRight>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);

	Mat left = imread(Constants::workDir + configuration.left);
	Mat dispLeft = imread(Constants::workDir + configuration.dispLeft, CV_LOAD_IMAGE_ANYDEPTH);
	Mat dispTruthLeft = imread(Constants::workDir + configuration.dispTruthLeft, CV_LOAD_IMAGE_ANYDEPTH);
	Mat dispTruthRight = imread(Constants::workDir + configuration.dispTruthRight, CV_LOAD_IMAGE_ANYDEPTH);

	const Mat texturedMask = BitmaskCreator::getTexturedPixels(left);
	const Mat occludedMask = BitmaskCreator::getOccludedPixels(dispTruthLeft, dispTruthRight);
	const Mat depthDiscontinuityMask = BitmaskCreator::getDepthDiscontinuedPixels(dispTruthLeft);
	const Mat salientMask = BitmaskCreator::getSalientPixels(left);
	const Mat unknownMask = BitmaskCreator::getUnknownDisparityPixels(dispLeft);
	imshow("unknownMask", unknownMask);
	imshow("texturedMask", texturedMask);
	imshow("occludedMask", occludedMask);
	imshow("salientMask", salientMask);
	imshow("depthDiscontinuityMask", depthDiscontinuityMask);
	waitKey(0);

	// TODO save bitmasks
	imwrite(Constants::workDir + "bitmask-textured.png", texturedMask);
	imwrite(Constants::workDir + "bitmask-occluded.png", occludedMask);
	imwrite(Constants::workDir + "bitmask-unknown.png", unknownMask);
	imwrite(Constants::workDir + "bitmask-salient.png", salientMask);
	imwrite(Constants::workDir + "bitmask-depthDiscontinuityMask.png", depthDiscontinuityMask);

	return 0;
}
