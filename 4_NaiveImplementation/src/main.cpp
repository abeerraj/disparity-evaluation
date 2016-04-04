#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include "Configuration.hpp"
#include "SimpleStereoMatcher.hpp"

using namespace std;
using namespace cv;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	string left = argv[1];
	string right = argv[2];
	string out = argv[3];
	configuration = {left, right, out};
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		cout << "Usage: " << argv[0] << " <left> <right> <out>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);

	Mat left = imread(configuration.left, CV_LOAD_IMAGE_GRAYSCALE);
	Mat right = imread(configuration.right, CV_LOAD_IMAGE_GRAYSCALE);

	int dMin = 0;
	int dMax = 16;
	int windowSize = 9;

	SimpleStereoMatcher matcher = SimpleStereoMatcher();

	Mat dsi = matcher.createDisparitySpaceImage(left, right, windowSize, dMin, dMax);
	Mat dispMap = matcher.getDisparityMap(dsi);
	imwrite(configuration.out, dispMap);
	return 0;
}
