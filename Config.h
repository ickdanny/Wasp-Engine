#pragma once

#include "framework.h"

namespace config {
	//window
	constexpr wchar_t className[]{ L"WSWP" };
	constexpr wchar_t windowName[]{ L"WSWP" };
	constexpr DWORD windowStyle{ WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU };
	constexpr int windowWidth{ 640 };
	constexpr int windowHeight{ 480 };

	//graphics
	const D2D1::ColorF fillColor{ D2D1::ColorF::Magenta };
	const D2D1::ColorF textColor{ D2D1::ColorF::White };
	constexpr wchar_t fontName[]{ L"Verdana" };
	constexpr float fontSize{ 12.0f };
	constexpr DWRITE_FONT_WEIGHT fontWeight{ DWRITE_FONT_WEIGHT_BOLD };
	constexpr DWRITE_FONT_STYLE fontStyle{ DWRITE_FONT_STYLE_NORMAL };
	constexpr DWRITE_FONT_STRETCH fontStretch{ DWRITE_FONT_STRETCH_NORMAL };
	constexpr DWRITE_TEXT_ALIGNMENT textAlignment{ DWRITE_TEXT_ALIGNMENT_LEADING };
	constexpr DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment{
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR
	};
}