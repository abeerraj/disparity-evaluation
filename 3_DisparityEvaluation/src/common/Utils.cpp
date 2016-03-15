#include "Utils.hpp"
#include "Constants.hpp"

// Stephan Schaber - SVDDD paper
const cv::Mat Utils::depth2disparity(const cv::Mat depth,
                                     double baseline_separation,
                                     double zero_disp_dist,
                                     double render_width,
                                     double focal_length,
                                     double sensor_width) {
	double tanFovBy2 = sensor_width /
	                   (2.0 * focal_length);

	double delta = (baseline_separation * render_width) /
	               (2.0 * zero_disp_dist * tanFovBy2);

	if (Constants::debug) std::cout << "delta = " << delta << std::endl;
	cv::Mat disparity = delta * (zero_disp_dist / depth - 1.0);
	return disparity;
}
