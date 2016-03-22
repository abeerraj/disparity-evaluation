#include "ELASAlgorithm.hpp"
#include "PfmReader.hpp"

/*
  usage: ./elas im0.pgm im1.pgm disp.pfm maxdisp [no_interp=0]
*/
void ELASAlgorithm::compute(const int identifier) {
	std::string output = Constants::tmpDir + std::to_string(identifier) + "_output-elas.pfm";
	std::string imgLpgm = Constants::tmpDir + std::to_string(identifier) + "_imgL-elas.pgm";
	std::string imgRpgm = Constants::tmpDir + std::to_string(identifier) + "_imgR-elas.pgm";
	std::string options = "64";

	cv::Mat imgL = cv::imread(this->imgL, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat imgR = cv::imread(this->imgR, CV_LOAD_IMAGE_GRAYSCALE);

	cv::imwrite(imgLpgm, imgL);
	cv::imwrite(imgRpgm, imgR);

	std::remove(output.c_str());

	std::string cmd = Constants::elasBinary;
	cmd += " " + imgLpgm;
	cmd += " " + imgRpgm;
	cmd += " " + output;
	cmd += " " + options;

	system(cmd.c_str());
	cv::Mat mat = PfmReader::loadPfm(output);
	mat.convertTo(this->result, CV_32F, 1.0 / 64.0);
}
