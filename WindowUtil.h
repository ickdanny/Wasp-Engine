#pragma once

#include "framework.h"

namespace wasp::window {
	MONITORINFO getPrimaryMonitorInfo();
	HMONITOR getPrimaryMonitorHandle();

	MONITORINFO getMonitorInfoFromWindow(HWND windowHandle);
	HMONITOR getMonitorHandleFromWindow(HWND windowHandle);

	int getWindowBorderWidthPadding();
	int getWindowBorderHeightPadding();
}