#pragma once

#if defined(MINISPRING_STATIC)
#define MINISPRING_API
#elif defined(_WIN32)
#ifdef MINISPRING_EXPORTS
#define MINISPRING_API __declspec(dllexport)
#else
#define MINISPRING_API __declspec(dllimport)
#endif
#else
#define MINISPRING_API __attribute__((visibility("default")))
#endif