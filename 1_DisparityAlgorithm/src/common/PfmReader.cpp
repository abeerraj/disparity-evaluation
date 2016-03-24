#include "PfmReader.hpp"
#include "Constants.hpp"

using namespace std;
using namespace cv;

Mat PfmReader::loadPfm(const string filename) {
	FILE *fp = fopen(filename.c_str(), "rb");
	if (fp == NULL) {
		cout << "PFM-File not found!" << endl;
		return Mat::zeros(0, 0, 0);
	}

	char pf[2];
	unsigned int width, height;
	float scaleFactor;

	// read header
	fscanf(fp, "%s\n%u %u\n", pf, &width, &height);
	fscanf(fp, "\n%f\n", &scaleFactor);

	// ELAS only produces grayscale PFM files
	if (strcmp(pf, "PF") == 0) {
		cout << "color PFM not supported" << endl;
		return Mat1f();
	}

	if (Constants::debug) {
		cout << "Keyword: " << pf << "(Grayscale PFM)" << endl;
		cout << "Size: " << width << "x" << height << endl;
		cout << "Scale factor: " << scaleFactor << endl;
	}

	// check for big endian
	if (scaleFactor > 0.0f) {
		// should never happen as ELAS produces little endian PFM files
		cout << "big endian PFM files not supported" << endl;
		return Mat1f();
	}

	Mat result(Size(width, height), CV_32FC1);
	float *data = (float *) result.data;

	// invert scale factor
	scaleFactor = -1.0f / scaleFactor;

	// read bottom-to-top
	int readPixels = 0;
	for (int y = height - 1; y >= 0; y--) {
		size_t n = fread(data + width * y, sizeof(float), width, fp);
		readPixels += width;
		if (n != width) {
			cout << "LoadPGM : fail to read pixels in " << filename << endl;
		}
	}
	fclose(fp);

	if (readPixels != result.size().area()) {
		cout << "Not all pixels are read." << endl;
		return Mat1f();
	}

	// apply scale factor
	result *= scaleFactor;

	return result;
}
