#pragma once

#include "framework.h"

namespace windowadapter {
	MONITORINFO getPrimaryMonitorInfo();
	HMONITOR getPrimaryMonitorHandle();

	MONITORINFO getMonitorInfoFromWindow(HWND windowHandle);
	HMONITOR getMonitorHandleFromWindow(HWND windowHandle);
}