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
#if 0
	Mat dispTruthLeftTmp = imread(configuration.dispTruthLeft, CV_LOAD_IMAGE_GRAYSCALE);
	Mat dispTruthLeft;
	dispTruthLeftTmp.convertTo(dispTruthLeft, CV_32FC1, 1.0 / 4);
	Mat dispTruthRightTmp = imread(configuration.dispTruthRight, CV_LOAD_IMAGE_GRAYSCALE);
	Mat dispTruthRight;
	dispTruthRightTmp.convertTo(dispTruthRight, CV_32FC1, 1.0 / 4);
#else
	Mat dispTruthLeft = imread(configuration.dispTruthLeft, CV_LOAD_IMAGE_ANYDEPTH);
	Mat dispTruthRight = imread(configuration.dispTruthRight, CV_LOAD_IMAGE_ANYDEPTH);
#endif

	const Mat texturedMask = MaskCreator::getTexturedPixels(left);
	const Mat occludedMask = MaskCreator::getOccludedPixels(dispTruthLeft, dispTruthRight);
	const Mat depthDiscontinuityMask = MaskCreator::getDepthDiscontinuedPixels(dispTruthLeft);
	Mat salientMask;

#if 1
	salientMask = MaskCreator::getSalientPixels(left);
#else
	cout << "salient mask is disabled" << endl;
#endif

	if (Constants::debug) {
		double min, max;
		minMaxLoc(dispTruthLeft, &min, &max);
		cout << "min disparity of ground-truth data: " << min << endl;
		cout << "max disparity of ground-truth data: " << max << endl;
	}

	string prefix = configuration.left;
	prefix.erase(0, prefix.find_last_of("/") + 1);
	prefix.erase(prefix.find_last_of("."), string::npos);
	if (Constants::debug) {
		cout << "prefix=" << prefix << endl;
	}

	imwrite(configuration.out + "/" + prefix + "-mask-textured.png", texturedMask);
	imwrite(configuration.out + "/" + prefix + "-mask-occluded.png", occludedMask);
	if (salientMask.data) {
		imwrite(configuration.out + "/" + prefix + "-mask-salient.png", salientMask);
	}
	imwrite(configuration.out + "/" + prefix + "-mask-depth-discontinuity.png", depthDiscontinuityMask);

	return 0;
}
