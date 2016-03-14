#include "ELASAlgorithm.hpp"
#include "Constants.hpp"

/*
  usage: ./elas im0.pgm im1.pgm disp.pfm maxdisp [no_interp=0]
*/
void ELASAlgorithm::compute() {
	std::string output = "/Users/bjohn/tmp/output-elas.pfm";
	std::string options = "64";

	std::string cmd = Constants::elasBinary;
	cmd += " " + Constants::workingDirectory + this->imgL;
	cmd += " " + Constants::workingDirectory + this->imgR;
	cmd += " " + output;
	cmd += " " + options;

	system(cmd.c_str());
	cv::Mat mat = cv::imread(output, CV_LOAD_IMAGE_GRAYSCALE);
	this->result = mat;
}
