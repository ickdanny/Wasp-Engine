#pragma once

#include "Window/WindowMode.h"
#include "Window/WindowUtil.h"
#include "Config.h"

namespace wasp::game::windowmodes {
	constexpr window::WindowMode windowed{
		L"WINDOWED",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		0,
		[]() -> std::pair<int, int> {
			return {config::windowWidth, config::windowHeight};
		},
		window::findCenterPositionWithBorder
	};
}