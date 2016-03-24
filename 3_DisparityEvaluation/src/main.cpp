#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "Constants.hpp"
#include "Configuration.hpp"
#include "Metrics.hpp"
#include "Heatmap.hpp"

using namespace std;
using namespace cv;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	string dispLeft = argv[1];
	string dispTruthLeft = argv[2];
	string bitmask = argv[3];
	configuration = {dispLeft, dispTruthLeft, bitmask};
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " <dispLeft> <dispTruthLeft> <bitmask...>" << endl;
		exit(1);
	}

	parseCommandLineArguments(argv);

#if 0
	Mat dispLeft = imread(Constants::workDir + configuration.dispLeft);
	Mat dispTruthLeft = imread(Constants::workDir + configuration.dispTruthLeft);
	Mat bitmask = imread(Constants::workDir + configuration.bitmask);
#endif

	//string path = "/Users/bjohn/Desktop/middlebury/01_book/disparity/image0001.png";
	string path = "/Users/bjohn/Desktop/tsukuba/01_tsukuba/tsukuba-truedispL.png";
	Mat dispTruthLeftPng = imread(path, CV_LOAD_IMAGE_ANYDEPTH);
	Mat dispTruthLeft;
	dispTruthLeftPng.convertTo(dispTruthLeft, CV_32FC1, 1 / 16.0);

	double min, max;
	minMaxLoc(dispTruthLeft, &min, &max);
	cout << endl << "dispTruthLeft min: " << min << " max: " << max << endl << endl;
	cout << "M[0] = " << endl << " " << dispTruthLeft.row(0) << endl << endl;

	//path = "/Users/bjohn/Desktop/middlebury/01_book/computed/8/image0001.exr";
	path = "/Users/bjohn/Desktop/result.exr";
	Mat dispLeft = imread(path, CV_LOAD_IMAGE_ANYDEPTH);
	cout << "M[0] = " << endl << " " << dispLeft.row(0) << endl << endl;

/*	Mat bitmaskNoc = imread("/Users/bjohn/Desktop/thesis/resources/bitmask-occluded.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat bitmaskUnk = imread("/Users/bjohn/Desktop/thesis/resources/bitmask-unknown.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat bitmaskTex = imread("/Users/bjohn/Desktop/thesis/resources/bitmask-textured.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat bitmaskSalient = imread("/Users/bjohn/Desktop/thesis/resources/bitmask-salient.png", CV_LOAD_IMAGE_GRAYSCALE);

	// Mat bitmask = bitmaskNoc & (Scalar::all(255) - bitmaskTex) & bitmaskUnk;
	// Mat bitmask = Scalar::all(255) - bitmaskTex;
	Mat bitmask = bitmaskNoc;

	Mat deltaMask = dispTruthLeft - dispLeft;

	double minDelta, maxDelta;
	minMaxLoc(deltaMask, &minDelta, &maxDelta);
	Mat deltaHeat = Heatmap::generateHeatmap(deltaMask, minDelta, maxDelta, COLORMAP_SUMMER);
	imwrite("/Users/bjohn/Desktop/test-delta.png", deltaHeat);*/

	// empty bitmask
	Mat bitmask(dispLeft.rows, dispLeft.cols, CV_8UC3, Scalar(255, 255, 255));

	Mat heatmap = Heatmap::generateHeatmap(dispLeft, min, max, bitmask);
	imwrite("/Users/bjohn/Desktop/ts-test.png", heatmap);

	Mat heatmapT = Heatmap::generateHeatmap(dispTruthLeft, min, max, bitmask);
	imwrite("/Users/bjohn/Desktop/ts-testT.png", heatmapT);

	Mat outliersHeatmap = Heatmap::generateOutliersHeatmap(dispLeft, dispTruthLeft, min, max);
	imwrite("/Users/bjohn/Desktop/ts-test-outliers.png", outliersHeatmap);


	double rmse = Metrics::getRMSE(dispLeft, dispTruthLeft, bitmask);
	double badPixels = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, bitmask);

	// TODO take all available bitmasks x and output as csv
	cout << "RMSE: " << rmse << endl;
	cout << "BadPixels: " << badPixels << "%" << endl;

	return 0;
}
