#include <iostream>
#include <boost/program_options.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include "Utils.hpp"
#include "DisparityAlgorithm.hpp"
#include "OpenCVStereoSGBM.hpp"

cv::Mat depth2disparity(const cv::Mat depth,
                    double baseline_separation,
                    double zero_disp_dist,
                    double render_width,
                    double focal_length,
                    double sensor_width = 32)
{
	double tanFovBy2 = sensor_width /
	                   (2.0 * focal_length);

	double delta = (baseline_separation * render_width) /
	               (2.0 * zero_disp_dist * tanFovBy2);

	std::cout << "   delta = " << delta << std::endl;

	cv::Mat disparity = delta * (zero_disp_dist / depth - 1.0);

	return disparity;
}

int main(int argc, const char *argv[]) {
	cv::Mat image = cv::imread("/Users/bjohn/Desktop/thesis/resources/03_LRview_LRdepth_cleaned/z-buf_L/Image0001.exr", CV_LOAD_IMAGE_ANYDEPTH);

	double baseline_separation = 25.8 / 1000;
	double zero_disp_dist = 2.7;
	double render_width = 1920;
	double focal_length = 60;

	cv::Mat trueDisparity = depth2disparity(image, baseline_separation, zero_disp_dist, render_width, focal_length);

	cv::Mat normalizedDisp;
	cv::normalize(trueDisparity, normalizedDisp, 0, 255, CV_MINMAX, CV_8UC1);

	cv::Mat t;
	trueDisparity.convertTo(t, CV_8U, 2.0);
	trueDisparity = t;

	double min, max;
	cv::minMaxLoc(normalizedDisp, &min, &max);
	std::cout << "normalizedDisp min: " << min << " max: " << max << std::endl;

	cv::minMaxLoc(trueDisparity, &min, &max);
	std::cout << "trueDisparity min: " << min << " max: " << max << std::endl;

	cv::imshow("trueDisparity", trueDisparity);
	cv::imshow("normalizedDisp", normalizedDisp);

	//cv::Mat left = cv::imread("/Users/bjohn/Desktop/thesis/resources/03_LRview_LRdepth_cleaned/left/Image0001.png", CV_LOAD_IMAGE_COLOR);
	//cv::Mat right = cv::imread("/Users/bjohn/Desktop/thesis/resources/03_LRview_LRdepth_cleaned/right/Image0001.png", CV_LOAD_IMAGE_COLOR);

#if 1
	// Load the image data from binary format
	std::ifstream is("/Users/bjohn/Desktop/test.exr",std::ios::in|std::ios::binary);
	if(!is.is_open())
	    return false;
	int rows,cols,type;
	is >> rows; is.ignore(1);
	is >> cols; is.ignore(1);
	is >> type; is.ignore(1);
	cv::Mat exrFileRead;
		exrFileRead.create(rows,cols,type);
	is.read((char*)exrFileRead.data,exrFileRead.step.p[0]*exrFileRead.rows);
	is.close();

	std::cout << "M = "<< std::endl << " "  << exrFileRead.row(0) << std::endl << std::endl;
	return 0;
#endif

#if 1

	cv::Mat img = cv::imread("/Users/bjohn/Desktop/TL0001.png", CV_LOAD_IMAGE_COLOR);
	cv::Mat test;
	img.convertTo(test, CV_32F, 1.0 / 4.0);
	cv::minMaxLoc(test, &min, &max);
	std::cout << "disp min: " << min << " max: " << max << std::endl;
	std::cout << "M = "<< std::endl << " "  << test.row(0) << std::endl << std::endl;

#endif

	cv::Mat left = cv::imread("/Users/bjohn/Desktop/thesis/resources/test/L0001.png", CV_LOAD_IMAGE_COLOR);
	cv::Mat right = cv::imread("/Users/bjohn/Desktop/thesis/resources/test/R0001.png", CV_LOAD_IMAGE_COLOR);

	DisparityAlgorithm *stereoSGBM = new OpenCVStereoSGBM(left, right);
	stereoSGBM->compute();

	cv::Mat result = stereoSGBM->getNormalizedResult();
	imshow("computed disp", result);

	cv::waitKey(0);

	std::string path = "/Users/bjohn/Desktop/thesis/resources/03_LRview_LRdepth_cleaned/";
	std::vector<Frame> frames = Utils::getAllFramesFromPath(path);

	return 0;

	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()
			("algorithm", po::value<std::string>()->required(),
			 "Algorithm to execute, possible values:\nStereoBM, StereoSGBM")
			("dataset", po::value<std::string>()->required(), "Dataset from which the sequence is selected")
			("sequence", po::value<std::string>()->required(), "Sequence on which to execute the algorithm")
			("help", "Print help messages");

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
	} catch (po::error &e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 1;
	}

	if (vm.count("algorithm")) {
		std::string algorithm = vm["algorithm"].as<std::string>();
		std::cout << "Algorithm:" << std::endl;
		std::cout << algorithm << std::endl;
	}
	if (vm.count("dataset")) {
		std::string dataset = vm["dataset"].as<std::string>();
		std::cout << "Dataset:" << std::endl;
		std::cout << dataset << std::endl;
	}
	if (vm.count("sequence")) {
		std::string sequence = vm["sequence"].as<std::string>();
		std::cout << "Sequence:" << std::endl;
		std::cout << sequence << std::endl;
	}
	if (vm.count("help")) {
		std::cout << "Basic Command Line Parameter App" << std::endl;
		std::cout << desc << std::endl;
		return 0;
	}

	try {
		po::notify(vm);
	} catch (po::error &e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 1;
	}

	// TODO execute chosen algorithm with sequence of dataset

	return 0;
}
