#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "Constants.hpp"
#include "Utils.hpp"
#include "Configuration.hpp"

const Configuration parseCommandLineArguments(const char *argv[]) {
	std::string left = argv[1];
	std::string dispTruthLeft = argv[2];
	std::string dispTruthRight = argv[3];
	return Configuration(left, dispTruthLeft, dispTruthRight);
}

int main(int argc, const char *argv[]) {
	if (argc < 4) {
		std::cout << "Usage: " << argv[0] << " <left> <dispTruthLeft> <dispTruthRight>" << std::endl;
		exit(1);
	}

	cv::Mat image = cv::imread("/Users/bjohn/Desktop/thesis/resources/03_LRview_LRdepth_cleaned/z-buf_L/Image0001.exr", CV_LOAD_IMAGE_ANYDEPTH);

	double baseline_separation = 25.8 / 1000;
	double zero_disp_dist = 2.7;
	double render_width = 1920;
	double focal_length = 60;

	cv::Mat trueDisparity = Utils::depth2disparity(image, baseline_separation, zero_disp_dist, render_width, focal_length);
	double min, max;
	cv::minMaxLoc(trueDisparity, &min, &max);
	std::cout << "trueDisparity min: " << min << " max: " << max << std::endl;

	const Configuration configuration = parseCommandLineArguments(argv);
	cv::Mat left = cv::imread(Constants::workDir + configuration.left);
	cv::Mat dispTruthLeft = cv::imread(Constants::workDir + configuration.dispTruthLeft);
	cv::Mat dispTruthRight = cv::imread(Constants::workDir + configuration.dispTruthRight);

	std::cout << "Hello world" << std::endl;

	// TODO write output

	return 0;
}
