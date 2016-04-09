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

Mat generateHeatmap(Mat disp, double min, double max, int colormap) {
	Mat adjMap;
	double scale = 255 / (max - min);
	disp.convertTo(adjMap, CV_8UC1, scale, -min * scale);

	Mat heatmap;
	applyColorMap(adjMap, heatmap, colormap);

	return heatmap;
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		cout << "Usage: " << argv[0] << " <left> <right> <out>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);

	Mat left = imread(configuration.left, CV_LOAD_IMAGE_GRAYSCALE);
	Mat right = imread(configuration.right, CV_LOAD_IMAGE_GRAYSCALE);

	int dMax = 64;
	int windowSize = 9;

	SimpleStereoMatcher matcher = SimpleStereoMatcher();

	Mat dsi = matcher.createDisparitySpaceImage(left, right, windowSize, dMax);
	Mat dispMap = matcher.getDisparityMap(dsi);


	// custom visualization
	Mat heatmap = generateHeatmap(dispMap, 0, 16, COLORMAP_AUTUMN);
	imshow("dispMap", heatmap);
	waitKey(0);

	imwrite(configuration.out, dispMap);
	return 0;
}
