#include <iostream>
#include "ELASAlgorithm.hpp"
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
	const int algorithmId = configuration.algorithmId;
	const std::string left = configuration.left;
	const std::string right = configuration.right;

	DisparityAlgorithm *algorithm = nullptr;

	// basic opencv stereo matcher
	if (algorithmId == 0) {
		algorithm = new OpenCVStereoSGBM(left, right);
	}
	if (algorithmId == 1) {
		algorithm = new OpenCVStereoBM(left, right);
	}

	// efficient large-scale stereo matcher
	if (algorithmId == 2) {
		algorithm = new ELASAlgorithm(left, right);
	}

	// MRFStereo variants
	if (algorithmId == 3) {
		algorithm = new MRFStereo(left, right, 0);
	}
	if (algorithmId == 4) {
		algorithm = new MRFStereo(left, right, 1);
	}
	if (algorithmId == 5) {
		algorithm = new MRFStereo(left, right, 2);
	}
	if (algorithmId == 6) {
		algorithm = new MRFStereo(left, right, 3);
	}
	if (algorithmId == 7) {
		algorithm = new MRFStereo(left, right, 4);
	}
	if (algorithmId == 8) {
		algorithm = new MRFStereo(left, right, 5);
	}

	return algorithm;
}

const cv::Mat executeAlgorithmWithConfiguration(const Configuration configuration) {
	DisparityAlgorithm *algorithm = getAlgorithmFromConfiguration(configuration);
	algorithm->compute();
	return algorithm->getResult();
}

void printDebugLogFromResultMat(const cv::Mat result) {
	if (!Constants::debug) return;
	cv::Mat out = result;

	// mask unkown disparity
	out.setTo(NAN, out == -1);
	out.setTo(NAN, out == 0);
	cv::SparseMat S = cv::SparseMat(out);

	// can still contain outliers from disparity calculation
	double min, max;
	cv::minMaxLoc(S, &min, &max);

	std::cout << "computed disparity min: " << min << std::endl;
	std::cout << "computed disparity max: " << max << std::endl;
	// print out the Mat
	std::cout << "M[0] = " << std::endl << " " << out.row(0) << std::endl << std::endl;
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
	saveResultMat(result, configuration.out);
	if (Constants::debug) printDebugLogFromResultMat(result);
	return 0;
}
