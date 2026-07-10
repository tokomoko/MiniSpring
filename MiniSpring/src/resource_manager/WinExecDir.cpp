#include "Platform.h"

#include"../core/Core.h"

#include <windows.h>

MINISPRING_API std::filesystem::path executableDirectory() {
	char buffer[MAX_PATH];
	DWORD size = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	return std::filesystem::path(std::string(buffer, size)).parent_path();
}