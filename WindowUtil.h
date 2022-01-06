#pragma once

#include "framework.h"

namespace wasp::windowadapter {
	MONITORINFO getPrimaryMonitorInfo();
	HMONITOR getPrimaryMonitorHandle();

	MONITORINFO getMonitorInfoFromWindow(HWND windowHandle);
	HMONITOR getMonitorHandleFromWindow(HWND windowHandle);

	int getWindowBorderWidthPadding();
	int getWindowBorderHeightPadding();
}