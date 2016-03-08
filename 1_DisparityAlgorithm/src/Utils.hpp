#pragma once

#include "Frame.hpp"

class Utils {
public:
	static std::vector<Frame> getAllFramesFromPath(std::string path);

	static void execute(std::string cmd);
};
