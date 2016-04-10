#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <memory>
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
using namespace chrono;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	string left = argv[1];
	string right = argv[2];
	string out = argv[3];
	int count = atoi(argv[4]);
	bool temporal = atoi(argv[5]) == 1;
	bool weighted = atoi(argv[6]) == 1;
	configuration = {left, right, out, count, temporal, weighted};
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

void const saveRuntime(const long long duration) {
	string f = configuration.out;
	ofstream out(f + "/runtime.txt");
	out << duration;
	out.close();
}

int main(int argc, const char *argv[]) {
	if (argc < 6) {
		cout << "Usage: " << argv[0] << " <left> <right> <out> <count> <temporal> <weighted>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);
	vector<string> images = createInputImageArray();

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	// create spatio-temporal DSI
	SimpleStereoMatcher matcher = SimpleStereoMatcher(configuration, images);

	for (int i = 0; i < configuration.count; i++) {
		string image = images[i];
		cout << "save image: " << image << ".exr" << endl;
		Mat dispMap = matcher.getDisparityMap(i);
		imwrite(configuration.out + "/" + image + ".exr", dispMap);
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	long long duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count() / 1000;
	cout << "duration: " << duration << " ms" << endl;
	saveRuntime(duration);

	return 0;
}
