#pragma once

#include "framework.h"

namespace wasp::config {
	//window
	constexpr wchar_t className[]{ L"WSWP" };
	constexpr wchar_t windowName[]{ L"WSWP" };
	constexpr DWORD windowStyle{ WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU };
	constexpr int windowWidth{ 640 };
	constexpr int windowHeight{ 480 };

	//data
	constexpr wchar_t mainManifestPath[]{ L"res\\main.mnfst" };

	//graphics
	constexpr int graphicsWidth{ windowWidth / 2 };
	constexpr int graphicsHeight{ windowHeight / 2 };
	constexpr int fillColor{ D2D1::ColorF::Black };
	constexpr int textColor{ D2D1::ColorF::White };
	constexpr wchar_t fontName[]{ L"Courier New" };
	constexpr float fontSize{ 11.0f };
	constexpr DWRITE_FONT_WEIGHT fontWeight{ DWRITE_FONT_WEIGHT_THIN };
	constexpr DWRITE_FONT_STYLE fontStyle{ DWRITE_FONT_STYLE_NORMAL };
	constexpr DWRITE_FONT_STRETCH fontStretch{ DWRITE_FONT_STRETCH_NORMAL };
	constexpr DWRITE_TEXT_ALIGNMENT textAlignment{ DWRITE_TEXT_ALIGNMENT_LEADING };
	constexpr DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment{
		DWRITE_PARAGRAPH_ALIGNMENT_NEAR
	};

	//game
	constexpr int updatesPerSecond{ 60 };
	constexpr int maxUpdatesWithoutFrame{ 5 };
}