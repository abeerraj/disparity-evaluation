#include "PfmReader.hpp"
#include "Constants.hpp"

cv::Mat PfmReader::loadPfm(const std::string filename) {
	std::FILE *fp = std::fopen(filename.c_str(), "rb");
	if (fp == NULL) {
		std::cout << "PFM-File not found!" << std::endl;
		return cv::Mat::zeros(0, 0, 0);
	}

	char pf[2];
	unsigned int width, height;
	float scaleFactor;

	// read header
	std::fscanf(fp, "%s\n%u %u\n", pf, &width, &height);
	std::fscanf(fp, "\n%f\n", &scaleFactor);

	// ELAS only produces grayscale PFM files
	if (strcmp(pf, "PF") == 0) {
		std::cout << "color PFM not supported" << std::endl;
		return cv::Mat::zeros(0, 0, 0);
	}

	if (Constants::debug) {
		std::cout << "Keyword: " << pf << "(Grayscale PFM)" << std::endl;
		std::cout << "Size: " << width << "x" << height << std::endl;
		std::cout << "Scale factor: " << scaleFactor << std::endl;
	}

	// check for big endian
	if (scaleFactor > 0.0f) {
		// should never happen as ELAS produces little endian PFM files
		std::cout << "big endian PFM files not supported" << std::endl;
		return cv::Mat::zeros(0, 0, 0);
	}

	int numPixels = width * height;
	float *data = new float[numPixels];

	// invert scale factor
	scaleFactor = -1.0f / scaleFactor;

	// read bottom-to-top
	int readPixels = 0;
	for (int y = height - 1; y >= 0; y--) {
		size_t n = std::fread(data + width * y, sizeof(float), width, fp);
		readPixels += width;
		if (n != width) {
			std::cout << "LoadPGM : fail to read pixels in " << filename << std::endl;
		}
	}

	if (readPixels != numPixels) {
		std::cout << "Not all pixels are read." << std::endl;
		return cv::Mat::zeros(0, 0, 0);
	}

	// apply scale factor
	for (int i = 0; i < numPixels; i++) data[i] *= scaleFactor;

	std::fclose(fp);

	cv::Mat result(cv::Size(width, height), CV_32FC1, data);
	delete data;
	return result;
}
