#pragma once

#include <iostream>
#include "Properties.hpp"

class LocalConfig : public Properties {
public:
	LocalConfig();

	std::string tmpDir();

	std::string mrfstereoBinary();

	std::string elasBinary();
};
