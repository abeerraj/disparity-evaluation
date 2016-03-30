#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <common/Constants.hpp>
#include <opencv2/highgui.hpp>
#include "MaskCreator.hpp"
#include "Configuration.hpp"

using namespace std;
using namespace cv;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	string left = argv[1];
	string dispTruthLeft = argv[2];
	string dispTruthRight = argv[3];
	string out = argv[4];
	configuration = {left, dispTruthLeft, dispTruthRight, out};
}

int main(int argc, const char *argv[]) {
	if (argc < 5) {
		cout << "Usage: " << argv[0] << " <left> <dispTruthLeft> <dispTruthRight> <out>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);

	Mat left = imread(configuration.left);
	Mat dispTruthLeft = imread(configuration.dispTruthLeft, CV_LOAD_IMAGE_ANYDEPTH);
	dispTruthLeft.convertTo(dispTruthLeft, CV_32FC1, 1.0 / 4);
	Mat dispTruthRight = imread(configuration.dispTruthRight, CV_LOAD_IMAGE_ANYDEPTH);
	dispTruthRight.convertTo(dispTruthRight, CV_32FC1, 1.0 / 4);

	const Mat texturedMask = MaskCreator::getTexturedPixels(left);
	const Mat occludedMask = MaskCreator::getOccludedPixels(dispTruthLeft, dispTruthRight);
	const Mat depthDiscontinuityMask = MaskCreator::getDepthDiscontinuedPixels(dispTruthLeft);
	const Mat salientMask = MaskCreator::getSalientPixels(left);

	if (Constants::debug) {
		double min, max;
		minMaxLoc(dispTruthLeft, &min, &max);
		cout << "min disparity of ground-truth data: " << min << endl;
		cout << "max disparity of ground-truth data: " << max << endl;
	}

	string prefix = configuration.left;
	prefix.erase(0, prefix.find_last_of("/") + 1);
	prefix.erase(prefix.find_last_of("."), string::npos);
	cout << "prefix=" << prefix << endl;

	imwrite(configuration.out + "/" + prefix + "-mask-textured.png", texturedMask);
	imwrite(configuration.out + "/" + prefix + "-mask-occluded.png", occludedMask);
	imwrite(configuration.out + "/" + prefix + "-mask-salient.png", salientMask);
	imwrite(configuration.out + "/" + prefix + "-mask-depth-discontinuity.png", depthDiscontinuityMask);

	return 0;
}
