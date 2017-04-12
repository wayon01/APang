#pragma once
#include <windows.h>
#include <string>

#define ID_NONE 0
#define ID_INT 1
#define ID_BOOL 2
#define ID_STRING 3

struct TileDetail {
	HWND name = nullptr;
	HWND value = nullptr;
	int id = ID_NONE;
	std::string strName;
};
