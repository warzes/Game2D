#pragma once

#include "PlatformDefine.h"

#if defined(_MSC_VER)
#	pragma warning(push, 3)
//#	pragma warning(disable : 5039)
#endif

#include <cstdint>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>

#if PLATFORM_WIN32
#endif // PLATFORM_WIN32

#if PLATFORM_EMSCRIPTEN
#endif // PLATFORM_EMSCRIPTEN

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif