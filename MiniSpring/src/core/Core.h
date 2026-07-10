#pragma once

#ifdef MINISPRING_BUILD_DLL
#define MINISPRING_API __declspec(dllexport)
#else
#define MINISPRING_API __declspec(dllimport)
#endif
