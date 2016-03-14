#include "ELASAlgorithm.hpp"

/*
  usage: ./elas im0.pgm im1.pgm disp.pfm maxdisp [no_interp=0]
*/
void ELASAlgorithm::compute() {
	std::string output = Constants::tmpDir + "output-elas.pfm";
	std::string imgLpgm = Constants::tmpDir + "imgL-elas.pgm";
	std::string imgRpgm = Constants::tmpDir + "imgR-elas.pgm";
	std::string options = "256";

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
	cv::Mat mat = this->loadPfm(output);
	mat.convertTo(this->result, CV_32F, 1.0 / 16.0);
}

cv::Mat ELASAlgorithm::loadPfm(const std::string filename) const {
	float *data;
	char strPF[3];
	unsigned int width, height;
	float scaleFactor;

	FILE *file = std::fopen(filename.c_str(), "rb");

	if (file == NULL) {
		printf("PFM-File not found!\n");
		return cv::Mat::zeros(0, 0, 0);
	}

	std::fscanf(file, "%s\n%u %u\n", strPF, &width, &height);
	std::fscanf(file, "\n%f\n", &scaleFactor);

	/* get return or space */
	//std::fgetc(file);

	int numPixels = width * height;

	//DEBUG Ausgabe
	printf("Keyword: %s\n", strPF);
	printf("Size X: %d\n", width);
	printf("Size Y: %d\n", height);
	printf("scaleFactor: %f\n", scaleFactor);
	// ENDE Debug Ausgabe

	/* Check for big endian PFM file */
	if (scaleFactor > 0.0f) {
		// should never happen as ELAS produces little endian PFM files
		std::cout << "Big endian PFM files not supported" << std::endl;
	}

	data = new float[numPixels];

	scaleFactor = -1.0f / scaleFactor;

	size_t n = std::fread(data, sizeof(float), numPixels, file);
	if (n != numPixels) {
		printf("LoadPGM : fail to read pixels in %s!\n", filename.c_str());
	}

	for (int i=0; i<numPixels; i++) {
		data[i] *= scaleFactor;
	}

	std::fclose(file);

	cv::Mat outputMat(cv::Size(width, height), CV_32FC1, data);
	// delete data;
	return outputMat;
}
