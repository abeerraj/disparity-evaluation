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
	string out = argv[5];
	configuration = {left, dispLeft, dispTruthLeft, dispTruthRight, out};
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		cout << "Usage: " << argv[0] << " <left> <dispLeft> <dispTruthLeft> <dispTruthRight> <out>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);

	Mat left = imread(configuration.left);
	Mat dispLeft = imread(configuration.dispLeft, CV_LOAD_IMAGE_ANYDEPTH);
	Mat dispTruthLeft = imread(configuration.dispTruthLeft, CV_LOAD_IMAGE_ANYDEPTH);
	Mat dispTruthRight = imread(configuration.dispTruthRight, CV_LOAD_IMAGE_ANYDEPTH);

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

	// TODO save bitmasks in subfolders according to given output path
	imwrite(configuration.out + "bitmask-textured.png", texturedMask);
	imwrite(configuration.out + "bitmask-occluded.png", occludedMask);
	imwrite(configuration.out + "bitmask-unknown.png", unknownMask);
	imwrite(configuration.out + "bitmask-salient.png", salientMask);
	imwrite(configuration.out + "bitmask-depthDiscontinuityMask.png", depthDiscontinuityMask);

	return 0;
}
