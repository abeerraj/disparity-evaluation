#include "MRFStereo.hpp"
#include "Utils.hpp"
#include "Constants.hpp"

/*
  mrfstereo version 1.0

  usage: ./mrfstereo [options] imL imR dispL

  reads imL and imR (in png or pgm/ppm format)
  runs MRF stereo
  writes dispL (in png or pgm/ppm format), disparities corresponding to imL

  options:
  -n nD          disparity levels, by default 16 (i.e. disparites 0..15)
  -b             use Birchfield/Tomasi costs
  -s             use squared differences (absolute differences by default)
  -t trunc       truncate differences to <= 'trunc'
  -a MRFalg      0-ICM, 1-Expansion (default), 2-Swap, 3-TRWS, 4-BPS, 5-BPM, 9-all
  -e smoothexp   smoothness exponent, 1 (default) or 2, i.e. L1 or L2 norm
  -m smoothmax   maximum value of smoothness term (2 by default)
  -l lambda      weight of smoothness term (20 by default)
  -g gradThresh  intensity gradient cue threshold
  -p gradPenalty if grad < gradThresh, multiply smoothness (2 by default)
  -o outscale    scale factor for disparities (full range by default)
  -w             write parameter settings to dispL.txt
  -x             write timings to dispL.csv
  -q             quiet (turn off debugging output)
*/
void MRFStereo::compute() {
  std::string output = "/Users/bjohn/tmp/output.png";
  std::string options = "-n 64 -a 1 -q";

  std::string cmd = Constants::mrfstereoExecutable + " " + options;
  cmd += " " + Constants::resourcesDirectory + this->imgL;
  cmd += " " + Constants::resourcesDirectory + this->imgR;
  cmd += " " + output;

  Utils::execute(cmd);
  cv::Mat mat = cv::imread(output, CV_LOAD_IMAGE_GRAYSCALE);
  this->setResult(mat);
}
