#include "Utils.hpp"
#include <cstdio>
#include <memory>

// helper for executing an arbitrary file and returning the output
std::string Utils::execute(std::string cmd) {
  system(cmd.c_str());
  return "";
  // shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
  // if (!pipe) return "ERROR";
  // char buffer[128];
  // string result = "";
  // while (!feof(pipe.get())) {
  //   if (fgets(buffer, 128, pipe.get()) != NULL)
  //   result += buffer;
  // }
  // return result;
}
