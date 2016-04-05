#include "LocalConfig.hpp"

using namespace std;

LocalConfig::LocalConfig() : Properties("./1_DisparityAlgorithm/local.ini") { }

string LocalConfig::mrfstereoBinary() {
	return getProperty("mrfstereoBinary");
}

string LocalConfig::elasBinary() {
	return getProperty("elasBinary");
}

string LocalConfig::tmpDir() {
	return getProperty("tmpDir");
}
