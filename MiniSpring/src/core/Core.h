#pragma once

#ifdef MINISPRING_BUILD_DLL
#define MINISPRING_API __declspec(dllexport)
#elif MINISPRING_BUILD_STATIC
#define MINISPRING_API
#else
#define MINISPRING_API __declspec(dllimport)
#endif
