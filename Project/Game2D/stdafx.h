#pragma once

#include "PlatformDefine.h"

#if defined(_MSC_VER)
#	pragma warning(push, 3)
//#	pragma warning(disable : 5039)
#endif

#include <cstdint>
#include <cassert>
#include <filesystem>
#include <string>
#include <vector>


#if PLATFORM_WIN32
#	include <glad/gl.h>
#	include <GLFW/glfw3.h>
#endif // PLATFORM_WIN32

#if PLATFORM_EMSCRIPTEN
#endif // PLATFORM_EMSCRIPTEN

#include <stb/stb_image.h>
#include <stb/stb_truetype.h>

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif