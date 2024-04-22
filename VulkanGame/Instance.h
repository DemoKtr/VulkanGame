#pragma once
#include "config.h"

namespace vkInit {
	
	bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool debug);

	
	vk::Instance make_instance(bool debugMode, const char* appName);
	
}