#include <iostream>
#include <fstream>
#include <chrono>
#include "ELASAlgorithm.hpp"
#include "MRFStereo.hpp"
#include "OpenCVStereoBM.hpp"
#include "OpenCVStereoSGBM.hpp"
#include "Configuration.hpp"
#include "Local.hpp"

using namespace std;
using namespace chrono;
using namespace cv;

Configuration configuration;

void parseCommandLineArguments(const char *argv[]) {
	int identifier = atoi(argv[1]);
	int algorithmId = atoi(argv[2]);
	string left = argv[3];
	string right = argv[4];
	string out = argv[5];
	configuration = {identifier, algorithmId, left, right, out};
}

shared_ptr<DisparityAlgorithm> getAlgorithmFromConfiguration() {
	const int algorithmId = configuration.algorithmId;
	string left = configuration.left;
	string right = configuration.right;

	if (configuration.right.compare("stereo") == 0) {
		if (Constants::debug) {
			cout << "split up stereo image from svddd dataset" << endl;
		}
		Mat stereo = imread(configuration.left);
		Mat leftMat = Mat(stereo, Rect(0, 0, 1920, 1080));
		Mat rightMat = Mat(stereo, Rect(1920, 0, 1920, 1080));
		left = Constants::tmpDir + to_string(configuration.identifier) + "_left.png";
		right = Constants::tmpDir + to_string(configuration.identifier) + "_right.png";
		imwrite(left, leftMat);
		imwrite(right, rightMat);
	}

	shared_ptr<DisparityAlgorithm> algorithm;

	// basic opencv stereo matcher
	if (algorithmId == 0) {
		algorithm = make_shared<OpenCVStereoSGBM>(left, right);
	}
	if (algorithmId == 1) {
		algorithm = make_shared<OpenCVStereoBM>(left, right);
	}

	// efficient large-scale stereo matcher
	if (algorithmId == 2) {
		algorithm = make_shared<ELASAlgorithm>(left, right);
	}

	// MRFStereo variants
	if (algorithmId == 3) {
		algorithm = make_shared<MRFStereo>(left, right, 0);
	}
	if (algorithmId == 4) {
		algorithm = make_shared<MRFStereo>(left, right, 1);
	}
	if (algorithmId == 5) {
		algorithm = make_shared<MRFStereo>(left, right, 2);
	}
	if (algorithmId == 6) {
		algorithm = make_shared<MRFStereo>(left, right, 3);
	}
	if (algorithmId == 7) {
		algorithm = make_shared<MRFStereo>(left, right, 4);
	}
	if (algorithmId == 8) {
		algorithm = make_shared<MRFStereo>(left, right, 5);
	}

	return algorithm;
}

const Mat executeAlgorithm() {
	shared_ptr<DisparityAlgorithm> algorithm = getAlgorithmFromConfiguration();
	algorithm->compute(configuration.identifier);
	return algorithm->getResult();
}

void const saveResultMat(const Mat result) {
	const string filename = configuration.out;
	if (Constants::debug) cout << "save disparity as exr file: " << filename << endl;
	imwrite(filename, result);
}

void const saveRuntime(const long long duration) {
	string f = configuration.out;
	f.erase(f.find_last_of("."), string::npos);
	ofstream out(f + "_runtime.txt");
	out << duration;
	out.close();
}

int main(int argc, const char *argv[]) {
	if (argc < 6) {
		cout << "Usage: " << argv[0] << " <identifier> <algorithmId> <left> <right> <out>" << endl;
		exit(1);
	}

	Local local;
	ifstream cfg("/Users/bjohn/Desktop/local.ini");
	local.parse(cfg);
	exit(0);

	parseCommandLineArguments(argv);

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	const Mat result = executeAlgorithm();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	long long duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count() / 1000;

	if (Constants::debug) {
		cout << "duration: " << duration << " ms" << endl;
	}

	saveRuntime(duration);
	saveResultMat(result);

	return 0;
}
