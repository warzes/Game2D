#pragma once

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
#	define PLATFORM_WIN32 1
#else
#	define PLATFORM_WIN32 0
#endif

#ifdef __EMSCRIPTEN__
#	define PLATFORM_EMSCRIPTEN 1
#else
#	define PLATFORM_EMSCRIPTEN 0
#endif

