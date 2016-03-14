#include <iostream>
#include <fstream>
#include "MRFStereo.hpp"
#include "OpenCVStereoBM.hpp"
#include "OpenCVStereoSGBM.hpp"
#include "Constants.hpp"
#include "Configuration.hpp"

const Configuration parseCommandLineArguments(const char *argv[]) {
	std::stringstream convert(argv[1]);
	int algorithmId;
	if (!(convert >> algorithmId))
		algorithmId = 0;
	std::string left = argv[2];
	std::string right = argv[3];
	std::string out = argv[4];
	Configuration configuration = Configuration(algorithmId, left, right, out);
	return configuration;
}

DisparityAlgorithm *getAlgorithmFromConfiguration(const Configuration configuration) {
	cv::Mat left = cv::imread(configuration.left, CV_LOAD_IMAGE_COLOR);
	cv::Mat right = cv::imread(configuration.right, CV_LOAD_IMAGE_COLOR);
	DisparityAlgorithm *algorithm = nullptr;
	if (configuration.algorithmId == 0) {
		algorithm = new OpenCVStereoSGBM(left, right);
	}
	if (configuration.algorithmId == 1) {
		algorithm = new OpenCVStereoBM(left, right);
	}
	if (configuration.algorithmId == 2) {
		algorithm = new MRFStereo(configuration.left, configuration.right);
	}
	return algorithm;
}

const cv::Mat executeAlgorithmWithConfiguration(const Configuration configuration) {
	DisparityAlgorithm *algorithm = getAlgorithmFromConfiguration(configuration);
	algorithm->compute();
	return algorithm->getResult();
}

void printDebugLogFromResultMat(const cv::Mat result) {
	cv::Mat out = result;
	out.setTo(NAN, out == -1);
	cv::SparseMat S = cv::SparseMat(out);

	double min, max;
	cv::minMaxLoc(S, &min, &max);
	std::cout << "computed disparity min: " << min << std::endl;
	std::cout << "computed disparity max: " << max << std::endl;

	std::cout << "M = " << std::endl << " " << out.row(0) << std::endl << std::endl;
}

void const saveResultMat(const cv::Mat result, const std::string filename) {
	if (Constants::debug) std::cout << "save disparity as exr file: " << filename << std::endl;
	cv::imwrite(filename, result);
}

int main(int argc, const char *argv[]) {
	if (argc < 5) {
		std::cout << "Usage: " << argv[0] << " <algorithmId> <left> <right> <out>" << std::endl;
		exit(1);
	}

	const Configuration configuration = parseCommandLineArguments(argv);
	const cv::Mat result = executeAlgorithmWithConfiguration(configuration);
	if (Constants::debug) printDebugLogFromResultMat(result);
	saveResultMat(result, configuration.out);
	return 0;
}
