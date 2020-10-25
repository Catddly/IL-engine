#pragma once

#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

#include <chrono>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "IL/Core/Log.h"
#include "IL/Debug/Profiler.h"

#ifdef IL_PLATFORM_WINDOWS
	#include <Windows.h>
#endif 