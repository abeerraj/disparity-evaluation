#include <iostream>
#include <iomanip>
#include <vector>
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
	int count = atoi(argv[4]);
	configuration = {left, right, out, count};
}

Mat generateHeatmap(Mat disp, double min, double max, int colormap) {
	Mat adjMap;
	double scale = 255 / (max - min);
	disp.convertTo(adjMap, CV_8UC1, scale, -min * scale);

	Mat heatmap;
	applyColorMap(adjMap, heatmap, colormap);

	return heatmap;
}

vector<string> createInputImageArray() {
	vector<string> images;
	for (int i = 0; i < configuration.count; i++) {
		string image;
		stringstream ss;
		ss << setfill('0') << setw(4) << to_string(i + 1);
		ss >> image;
		image = "image" + image;
		images.push_back(image);
	}
	return images;
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		cout << "Usage: " << argv[0] << " <left> <right> <out> <count>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);
	vector<string> images = createInputImageArray();

	// create spatio-temporal DSI
	SimpleStereoMatcher matcher = SimpleStereoMatcher(configuration, images);

	for (int i = 0; i < configuration.count; i++) {
		cout << "save image: " << image << ".exr" << endl;
		string image = images[i];
		Mat dispMap = matcher.getDisparityMap(i);
		imwrite(configuration.out + "/" + image + ".exr", dispMap);
	}

#if 0
	// for Cambridge dataset
	string truthPath = "/Users/bjohn/Desktop/datasets/cambridge/01-book/disparity-left/image0001.png";
	Mat dispTruthLeftTmp = imread(truthPath, CV_LOAD_IMAGE_GRAYSCALE);
	Mat dispTruthLeft;
	dispTruthLeftTmp.convertTo(dispTruthLeft, CV_32FC1, 1 / 4.0);
	double min, max;
	minMaxLoc(dispTruthLeft, &min, &max);

	// custom visualization
	Mat heatmap = generateHeatmap(dispMap, min, max, COLORMAP_AUTUMN);
	imshow("dispMap", heatmap);
	waitKey(0);
#endif

	return 0;
}
