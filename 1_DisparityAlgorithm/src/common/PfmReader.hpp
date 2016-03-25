#pragma once

#include <iostream>
#include <opencv2/core/core.hpp>

// http://netpbm.sourceforge.net/doc/pfm.html
class PfmReader {
public:
	static cv::Mat loadPfm(const std::string filename);
};
