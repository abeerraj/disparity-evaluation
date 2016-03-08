#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "Constants.hpp"
#include "Utils.hpp"

std::vector<Frame> Utils::getAllFramesFromPath(std::string path) {
  std::vector<Frame> frames;
  boost::filesystem::directory_iterator end_itr;
  int counter = 1;
  for (boost::filesystem::directory_iterator itr(std::string(Constants::workingDirectory) + path); itr != end_itr; ++itr) {
    std::string filename = itr->path().filename().string();
    if (boost::algorithm::starts_with(filename, "L") && boost::algorithm::ends_with(filename, ".png")) {
      std::string base = filename.substr(1, filename.length());
      Frame frame = Frame();
      frame.id = counter++;
      frame.path = path;
      frame.base = base;
      frame.imgLeft = "L" + base;
      frame.imgRight = "R" + base;
      frame.truthLeft = "TL" + base;
      frame.truthRight = "TR" + base;
      frames.push_back(frame);
    }
  }
  return frames;
}

// helper for executing an arbitrary file and returning the output
void Utils::execute(std::string cmd) {
  system(cmd.c_str());
}
