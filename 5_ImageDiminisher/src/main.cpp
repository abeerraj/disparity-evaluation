#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include "Configuration.hpp"

using namespace std;
using namespace cv;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	string in = argv[1];
	string out = argv[2];
	double sigma = atof(argv[3]);
	configuration = {in, out, sigma};
}

uchar clamp(int p) {
	if (p > 256) return 255;
	if (p < 0) return 0;
	return (uchar) p;
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		cout << "Usage: " << argv[0] << " <in> <out> <sigma>" << endl;
		exit(1);
	}
	parseCommandLineArguments(argv);

	Mat in = imread(configuration.in);
	Mat noise(in.size(), CV_16SC3);
	randn(noise, 0, configuration.sigma);
	Mat out(in.size(), CV_8UC3);

	for (int row = 0; row < in.rows; row++) {
		for (int col = 0; col < in.cols; col++) {
			Vec3b sourcePixel = in.at<Vec3b>(row, col);
			Vec3s NoisePixel = noise.at<Vec3s>(row, col);
			Vec3b &DestPixel = out.at<Vec3b>(row, col);
			for (int i = 0; i < 3; i++) {
				DestPixel.val[i] = clamp(sourcePixel.val[i] + NoisePixel.val[i]);
			}
		}
	}

	imwrite(configuration.out, out);
	return 0;
}
