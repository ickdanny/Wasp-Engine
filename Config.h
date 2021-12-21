#pragma once

#include "framework.h"

namespace config {
	constexpr wchar_t className[]{ L"WSWP" };
	constexpr wchar_t windowName[]{ L"WSWP" };
	constexpr DWORD windowStyle{ WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU };

	constexpr int windowWidth{ 640 };
	constexpr int windowHeight{ 480 };
}