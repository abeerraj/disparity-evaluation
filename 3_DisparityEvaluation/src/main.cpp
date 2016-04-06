#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include "Constants.hpp"
#include "Configuration.hpp"
#include "Metrics.hpp"
#include "Heatmap.hpp"

using namespace std;
using namespace cv;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	string dispTruthLeft = argv[1];
	string dispLeft = argv[2];
	configuration = {dispTruthLeft, dispLeft};
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " <dispTruthLeft> <dispLeft>" << endl;
		exit(1);
	}

	parseCommandLineArguments(argv);

	// get algorithmId, path and prefix from dispLeftPath
	string algorithmId = configuration.dispLeft;
	algorithmId.erase(algorithmId.find_last_of("/"), string::npos);
	algorithmId.erase(0, algorithmId.find_last_of("/") + 1);
	if (Constants::debug) {
		cout << "algorithmId=" << algorithmId << endl;
	}

	string root = configuration.dispLeft;
	root.erase(root.find_last_of("/"), string::npos);
	root.erase(root.find_last_of("/"), string::npos);
	root.erase(root.find_last_of("/"), string::npos);
	root.erase(root.find_last_of("/") + 1, string::npos);
	if (Constants::debug) {
		cout << "root=" << root << endl;
	}

	string path = configuration.dispLeft;
	path.erase(path.find_last_of("/"), string::npos);
	path.erase(path.find_last_of("/"), string::npos);
	path.erase(path.find_last_of("/") + 1, string::npos);
	string masks = path + "masks/";
	string eval = path + "eval/" + algorithmId + "/";
	if (Constants::debug) {
		cout << "path=" << path << endl;
		cout << "masks=" << masks << endl;
		cout << "eval=" << eval << endl;
	}

	string prefix = configuration.dispLeft;
	prefix.erase(0, prefix.find_last_of("/") + 1);
	prefix.erase(prefix.find_last_of("."), string::npos);
	if (Constants::debug) {
		cout << "prefix=" << prefix << endl;
	}

	// load mats to compare
	Mat dispLeft = imread(configuration.dispLeft, CV_LOAD_IMAGE_ANYDEPTH);
	Mat dispTruthLeftTmp = imread(configuration.dispTruthLeft, CV_LOAD_IMAGE_GRAYSCALE);
	Mat dispTruthLeft;
	dispTruthLeftTmp.convertTo(dispTruthLeft, CV_32FC1, 1 / 4.0);

	// load masks
	Mat texturedMask = imread(masks + prefix + "-mask-textured.png", CV_LOAD_IMAGE_GRAYSCALE);
	texturedMask = 255 - texturedMask;
	const Mat occludedMask = imread(masks + prefix + "-mask-occluded.png", CV_LOAD_IMAGE_GRAYSCALE);
	const Mat depthDiscMask = imread(masks + prefix + "-mask-depth-discontinuity.png", CV_LOAD_IMAGE_GRAYSCALE);
	const Mat salientMask = imread(masks + prefix + "-mask-salient.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat borderMask = imread(root + "border-mask-" + algorithmId + ".png", CV_LOAD_IMAGE_GRAYSCALE);

	if (!borderMask.data) {
		cout << "no border mask, creating empty one" << endl;
		borderMask = Mat(texturedMask.size(), texturedMask.type(), Scalar::all(255));
	}

	// get (min,max) from truth for proper heatmap scaling
	double min, max;
	minMaxLoc(dispTruthLeft, &min, &max);
	if (Constants::debug) {
		cout << endl << "dispTruthLeft min: " << min << " max: " << max << endl << endl;
		cout << "dispTruthLeft[0] = " << endl << " " << dispTruthLeft.row(0) << endl << endl;
		cout << "dispLeft[0] = " << endl << " " << dispLeft.row(0) << endl << endl;
	}

	Mat heatmapGroundTruth = Heatmap::generateHeatmap(dispTruthLeft, min, max);
	imwrite(eval + prefix + "-heatmap-ground-truth.png", heatmapGroundTruth);

	Mat heatmapDisparity = Heatmap::generateHeatmap(dispLeft, min, max, borderMask);
	imwrite(eval + prefix + "-heatmap-disparity.png", heatmapDisparity);

	Mat heatmapOutliers = Heatmap::generateOutliersHeatmap(dispLeft, dispTruthLeft, borderMask, min, max);
	imwrite(eval + prefix + "-heatmap-outliers.png", heatmapOutliers);

	string f = eval + prefix + "_result.txt";
	ofstream out(f);
	out << "prefix;rmseAll;rmseDisc;rmseNoc;rmseTex;rmseSal;";
	out << "pbmpAll1;pbmpDisc1;pbmpNoc1;pbmpTex1;pbmpSal1;";
	out << "pbmpAll2;pbmpDisc2;pbmpNoc2;pbmpTex2;pbmpSal2;";
	out << "pbmpAll4;pbmpDisc4;pbmpNoc4;pbmpTex4;pbmpSal4;";
	out << endl;
	out << prefix << ";";

	double rmseAll = Metrics::getRMSE(dispLeft, dispTruthLeft, borderMask);
	double rmseDisc = Metrics::getRMSE(dispLeft, dispTruthLeft, depthDiscMask);
	double rmseNoc = Metrics::getRMSE(dispLeft, dispTruthLeft, borderMask & occludedMask);
	double rmseTex = Metrics::getRMSE(dispLeft, dispTruthLeft, borderMask & texturedMask);
	double rmseSal = Metrics::getRMSE(dispLeft, dispTruthLeft, salientMask);

	out << rmseAll << ";";
	out << rmseDisc << ";";
	out << rmseNoc << ";";
	out << rmseTex << ";";
	out << rmseSal << ";";

	double pbmpAll, pbmpDisc, pbmpNoc, pbmpTex, pbmpSal;
	float thresholds[] = {1.0f, 2.0f, 4.0f};
	for (float t : thresholds) {
		pbmpAll = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, borderMask, t);
		pbmpDisc = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, depthDiscMask, t);
		pbmpNoc = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, borderMask & occludedMask, t);
		pbmpTex = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, borderMask & texturedMask, t);
		pbmpSal = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, salientMask, t);

		out << pbmpAll << ";";
		out << pbmpDisc << ";";
		out << pbmpNoc << ";";
		out << pbmpTex << ";";
		out << pbmpSal << ";";
	}

	out << endl;
	out.close();

	return 0;
}
