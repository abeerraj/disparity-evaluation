#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "Constants.hpp"
#include "Utils.hpp"
#include "Configuration.hpp"
#include "Metrics.hpp"

const Configuration parseCommandLineArguments(const char *argv[]) {
	std::string dispLeft = argv[1];
	std::string dispTruthLeft = argv[2];
	std::string bitmask = argv[3];
	return Configuration(dispLeft, dispTruthLeft, bitmask);
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " <dispLeft> <dispTruthLeft> <bitmask...>" << std::endl;
		exit(1);
	}

#if 0
	const Configuration configuration = parseCommandLineArguments(argv);
	cv::Mat dispLeft = cv::imread(Constants::workDir + configuration.dispLeft);
	cv::Mat dispTruthLeft = cv::imread(Constants::workDir + configuration.dispTruthLeft);
	cv::Mat bitmask = cv::imread(Constants::workDir + configuration.bitmask);


	// parameters for sequences of SVDDD dataset
	double baseline_separation = 25.8 / 1000;
	double zero_disp_dist = 2.7;
	double render_width = 1920;
	double focal_length = 60;

	cv::Mat trueDisparity = Utils::depth2disparity(dispTruthLeft,
	                                               baseline_separation,
	                                               zero_disp_dist,
	                                               render_width,
	                                               focal_length);

	double min, max;
	cv::minMaxLoc(trueDisparity, &min, &max);
	std::cout << "trueDisparity min: " << min << " max: " << max << std::endl;
#endif

	std::string path = "/Users/bjohn/Desktop/thesis/resources/03_LRview_LRdepth_cleaned/z-buf_L/Image0001.exr";
	cv::Mat dispTruthLeft = cv::imread(path, CV_LOAD_IMAGE_ANYDEPTH);
	path = "/Users/bjohn/Desktop/result.exr";
	cv::Mat dispLeft = cv::imread(path, CV_LOAD_IMAGE_ANYDEPTH);
	cv::Mat bitmask = cv::Mat::zeros(dispTruthLeft.size(), dispTruthLeft.type());

	float rmse = Metrics::getRMSE(dispLeft, dispTruthLeft, bitmask);
	float badPixels = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, bitmask);

	std::cout << "RMSE: " << rmse << std::endl;
	std::cout << "BadPixels: " << badPixels << std::endl;
	std::cout << "Hello world" << std::endl;

	return 0;
}
