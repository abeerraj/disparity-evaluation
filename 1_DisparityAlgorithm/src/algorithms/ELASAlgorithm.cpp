#include "ELASAlgorithm.hpp"
#include "PfmReader.hpp"

using namespace std;
using namespace cv;

/*
  usage: ./elas im0.pgm im1.pgm disp.pfm maxdisp [no_interp=0]
*/
void ELASAlgorithm::compute(const int identifier) {
	string output = localConfig.tmpDir() + "/" + to_string(identifier) + "_output-elas.pfm";
	string imgLpgm = localConfig.tmpDir() + "/" + to_string(identifier) + "_imgL-elas.pgm";
	string imgRpgm = localConfig.tmpDir() + "/" + to_string(identifier) + "_imgR-elas.pgm";
	string options = "64";

	Mat imgL = imread(this->imgL, CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgR = imread(this->imgR, CV_LOAD_IMAGE_GRAYSCALE);

	imwrite(imgLpgm, imgL);
	imwrite(imgRpgm, imgR);

	remove(output.c_str());

	string cmd = localConfig.elasBinary();
	cmd += " " + imgLpgm;
	cmd += " " + imgRpgm;
	cmd += " " + output;
	cmd += " " + options;

	system(cmd.c_str());
	Mat mat = PfmReader::loadPfm(output);
	mat.convertTo(this->result, CV_32F, 1.0 / 64.0);
}
