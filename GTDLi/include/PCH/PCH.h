#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstdarg>
#include <stdio.h>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <array>
#include <numeric>

//#include <glm/glm.hpp>

#ifdef GTD_PLATFORM_WINDOWS
// windows specific
	#include <Windows.h>
	//#include "../../../Vendor/GLAD/include/glad/glad.h" // Vendor - load addresses for gl functions
	//#include <glfw3.h> // Vendor - window creation and window event system (mouse, resize, controllers...)
#else
	#include <stdlib.h> // Linux or Mac? 
#endif