#pragma once

#ifdef FF_PLATFORM_WINDOWS
	#ifdef FF_DYNAMIC_LINK
		#ifdef FF_BUILD_DLL
			#define FIREFLY_API __declspec(dllexport)
		#else
			#define FIREFLY_API __declspec(dllimport)
		#endif
	#else
		#define FIREFLY_API
	#endif
#else
	#error Firefly only support Windows
#endif

#ifdef FF_DEBUG
    #define FF_ENABLE_ASSERTS
#endif

#ifdef FF_ENABLE_ASSERTS
#define FF_ASSERT(x, ...) {if(!(x)) { FF_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define FF_CORE_ASSERT(x, ...) {if(!(x)) { FF_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define FF_ASSERT(x, ...)
    #define FF_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define FF_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
