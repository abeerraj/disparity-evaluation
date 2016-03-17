#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "Constants.hpp"
#include "Utils.hpp"
#include "Configuration.hpp"
#include "Metrics.hpp"
#include "Heatmap.hpp"

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
#endif

	std::string path = "/Users/bjohn/Desktop/thesis/resources/03_LRview_LRdepth_cleaned/z-buf_L/Image0001.exr";
	cv::Mat depthTruthLeft = cv::imread(path, CV_LOAD_IMAGE_ANYDEPTH);

	double min, max;
	cv::minMaxLoc(depthTruthLeft, &min, &max);
	std::cout << std::endl << "depthTruthLeft min: " << min << " max: " << max << std::endl << std::endl;

	// parameters for sequences of SVDDD dataset
	double baseline_separation = 25.8 / 1000;
	double zero_disp_dist = 2.7;
	double render_width = 1920;
	double focal_length = 32;

	// 10-tree scene
	baseline_separation = 5.0 / 1000;
	zero_disp_dist = 32;
	render_width = 1920;
	focal_length = 80;

	cv::Mat dispTruthLeft = Utils::depth2disparity(depthTruthLeft,
	                                               baseline_separation,
	                                               zero_disp_dist,
	                                               render_width,
	                                               focal_length);

	cv::minMaxLoc(dispTruthLeft, &min, &max);
	std::cout << "dispTruthLeft min: " << min << " max: " << max << std::endl << std::endl;

	path = "/Users/bjohn/Desktop/thesis/resources/result-sgbm.exr";
	cv::Mat dispLeft = cv::imread(path, CV_LOAD_IMAGE_ANYDEPTH);

	cv::Mat bitmaskNoc = cv::imread("/Users/bjohn/Desktop/thesis/resources/bitmask-occluded.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat bitmaskUnk = cv::imread("/Users/bjohn/Desktop/thesis/resources/bitmask-unknown.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat bitmaskTex = cv::imread("/Users/bjohn/Desktop/thesis/resources/bitmask-textured.png", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat bitmaskSalient = cv::imread("/Users/bjohn/Desktop/thesis/resources/bitmask-salient.png", CV_LOAD_IMAGE_GRAYSCALE);

	// cv::Mat bitmask = bitmaskNoc & (cv::Scalar::all(255) - bitmaskTex) & bitmaskUnk;
	// cv::Mat bitmask = cv::Scalar::all(255) - bitmaskTex;
	cv::Mat bitmask = bitmaskNoc;

	cv::Mat deltaMask = dispTruthLeft - dispLeft;

	double minDelta, maxDelta;
	cv::minMaxLoc(deltaMask, &minDelta, &maxDelta);
	cv::Mat deltaHeat = Heatmap::generateHeatmap(deltaMask, minDelta, maxDelta, cv::COLORMAP_SUMMER);
	cv::imwrite("/Users/bjohn/Desktop/test-delta.png", deltaHeat);

	cv::Mat heatmap = Heatmap::generateHeatmap(dispLeft, min, max, bitmask);
	cv::imwrite("/Users/bjohn/Desktop/test.png", heatmap);

	cv::Mat outliersHeatmap = Heatmap::generateOutliersHeatmap(dispLeft, dispTruthLeft, min, max);
	cv::imwrite("/Users/bjohn/Desktop/test-outliers.png", outliersHeatmap);

	double rmse = Metrics::getRMSE(dispLeft, dispTruthLeft, bitmask);
	double badPixels = Metrics::getPercentageOfBadPixels(dispLeft, dispTruthLeft, bitmask);

	std::cout << "RMSE: " << rmse << std::endl;
	std::cout << "BadPixels: " << badPixels << "%" << std::endl;

	return 0;
}
