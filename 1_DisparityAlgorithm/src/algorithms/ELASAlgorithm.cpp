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
	mat.convertTo(this->result, CV_32F, 1.0);
}

void ELASAlgorithm::read_comment(FILE *fp) const
{
	if (fgetc(fp) == '#')
	{
		while (fgetc(fp) != '\n');
		read_comment(fp);
	}
	else
	{
		fseek(fp, -1, SEEK_CUR);
	}
}

void ELASAlgorithm::SwapBytes(unsigned char* pixels, int nPixels, int pixDepth) const
{
	if (pixDepth == 1) return;
	if (pixDepth == 2)
	{
		unsigned char *p = pixels;
		unsigned char *pLast = pixels+nPixels*pixDepth;
		for (;p != pLast;p+= pixDepth)
		{
			unsigned short &a = *(unsigned short*)p;
			unsigned short b = 0;
			b |= ((a&0x00ff)<<8);
			b |= ((a&0xff00)>>8);
			a = b;

		}
	}
	else if (pixDepth == 4)
	{
		unsigned char *p = pixels;
		unsigned char *pLast = pixels+nPixels*pixDepth;
		for (;p != pLast;p+= pixDepth)
		{
			unsigned int &a = *(unsigned int*)p;
			unsigned int b = 0;
			b |= ((a&0x000000ff)<<24);
			b |= ((a&0x0000ff00)<<8);
			b |= ((a&0x00ff0000)>>8);
			b |= ((a&0xff000000)>>24);
			a = b;
		}
	}
	else
	{
		printf("SwapBytes: unsupported pixel depth!\n");
	}
}

cv::Mat ELASAlgorithm::loadPfm(const std::string filename) const {
	float *data;
	char strPF[3];
	unsigned int width, height;

	FILE *fp = std::fopen(filename.c_str(), "rb");

	if (fp == NULL) {
		printf("PFM-File not found!\n");
		return cv::Mat::zeros(0, 0, 0);
	}

	//read header

	int nbands;
	const char *pathname = filename.c_str();
	read_comment(fp);
	char magic[16];
	fscanf(fp, "%s", magic);
	if (strcmp(magic, "PF")==0)	nbands = 3;
	else if (strcmp(magic, "Pf")==0) nbands = 1;
	else
	{
		printf("LoadPFM : invalid magic number in %s!\n", pathname);
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%d", &width);
	if (width <= 0)
	{
		printf("LoadPFM : invalid width in %s!\n", pathname);
	}
	fgetc(fp);

	read_comment(fp);
	fscanf(fp, "%d", &height);
	if (height <= 0)
	{
		printf("LoadPFM : invalid height in %s!\n", pathname);
	}
	fgetc(fp);

	read_comment(fp);
	int maxval;
	fscanf(fp, "%f", &maxval);
	if (maxval == 0)
	{
		printf("LoadPFM : invalid maxval in %s\n", pathname);
	}
	fgetc(fp);

	/*std::fscanf(file, "%s\n%u %u\n", strPF, &width, &height);
	std::fscanf(file, "\n%f\n", &dummy);*/

	int numPixels = width * height;

	//DEBUG Ausgabe
	printf("Keyword: %s\n", magic);
	printf("Size X: %d\n", width);
	printf("Size Y: %d\n", height);
	// ENDE Debug Ausgabe

	data = new float[numPixels];

	size_t n = std::fread(data, sizeof(float), numPixels, fp);
	if (n != numPixels) {
		printf("LoadPGM : fail to read pixels in %s!\n", filename.c_str());
	}

	if (maxval>0) SwapBytes((unsigned char*)data, numPixels, sizeof(float)); //big endian
	else maxval = -maxval; //little endian

	std::fclose(fp);

	cv::Mat outputMat(cv::Size(width, height), CV_32FC1, data);
	delete data;
	return outputMat;
}
