#include "WindowUtil.h"

#include <stdexcept>

namespace windowadapter {
	MONITORINFO getPrimaryMonitorInfo()
	{
		MONITORINFO toRet{};
		toRet.cbSize = sizeof(toRet);//needed
		BOOL success{ GetMonitorInfo(getPrimaryMonitorHandle(), &toRet) };
		if (!success) {
			throw std::runtime_error{ "Error retrieving primary monitor info" };
		}
		return toRet;
	}
	HMONITOR getPrimaryMonitorHandle()
	{
		static constexpr POINT origin{ 0,0 };
		return MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
	}
	MONITORINFO getMonitorInfoFromWindow(HWND windowHandle) {
		MONITORINFO toRet{};
		toRet.cbSize = sizeof(toRet);//needed
		BOOL success{ 
			GetMonitorInfo(getMonitorHandleFromWindow(windowHandle), &toRet) 
		};
		if (!success) {
			throw std::runtime_error{ "Error retrieving monitor info" };
		}
		return toRet;
	}
	HMONITOR getMonitorHandleFromWindow(HWND windowHandle) {
		return MonitorFromWindow(windowHandle, MONITOR_DEFAULTTONEAREST);
	}
}