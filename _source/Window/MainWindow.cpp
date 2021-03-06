#include "Window\MainWindow.h"

#include "Logging.h"

namespace wasp::window {

	MainWindow::MainWindow(
		const WindowMode& initWindowMode,
		HINSTANCE instanceHandle,
		PCWSTR className,
		PCWSTR windowName,
		//graphics fields
		int graphicsWidth,
		int graphicsHeight,
		int fillColor,
		int textColor,
		wchar_t const* fontName,
		float fontSize,
		DWRITE_FONT_WEIGHT fontWeight,
		DWRITE_FONT_STYLE fontStyle,
		DWRITE_FONT_STRETCH fontStretch,
		DWRITE_TEXT_ALIGNMENT textAlignment,
		DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment
	)
		: currentWindowModeName{initWindowMode.modeName}
		, windowPainter{
				graphicsWidth,
				graphicsHeight,
				fillColor,
				textColor,
				fontName,
				fontSize,
				fontWeight,
				fontStyle,
				fontStretch,
				textAlignment,
				paragraphAlignment
		}
	{
		std::pair<int, int> size{ initWindowMode.sizeFunction() };
		std::pair<int, int> position{ initWindowMode.positionFunction(size) };

		create(
			instanceHandle,
			className,
			windowName,
			initWindowMode.windowStyle,
			initWindowMode.windowExtraStyle,
			position.first,
			position.second,
			size.first,
			size.second
		);
	}

	LRESULT MainWindow::handleMessage(UINT messageCode, WPARAM wParam, LPARAM lParam) {
		switch (messageCode)
		{
			case WM_CREATE: // gets recieved before main exits window.create
				windowPainter.init(windowHandle);
				return 0;

			case WM_PAINT:
				windowPainter.paint(windowHandle);
				return 0;

			case WM_SIZE:
				windowPainter.resize(windowHandle);
				return 0;
				
			case WM_KEYDOWN:
				keyDownCallback(wParam, lParam);
				return 0;

			case WM_KEYUP:
				keyUpCallback(wParam, lParam);
				return 0;

			case WM_KILLFOCUS:
			case WM_ENTERSIZEMOVE:
				outOfFocusCallback();
				return 0;
		
			case WM_DESTROY:
				destroyCallback();
				PostQuitMessage(0);
				return 0;

			default:
				return DefWindowProc(windowHandle, messageCode, wParam, lParam);
		}
		return TRUE;
	}

	void MainWindow::changeWindowMode(const WindowMode& windowMode) {
		if (currentWindowModeName != windowMode.modeName) {
			currentWindowModeName = windowMode.modeName;

			std::pair<int, int> size{ windowMode.sizeFunction() };
			std::pair<int, int> position{ windowMode.positionFunction(size) };

			SetWindowLong(windowHandle, GWL_STYLE, windowMode.windowStyle);
			SetWindowLong(windowHandle, GWL_EXSTYLE, windowMode.windowExtraStyle);
			SetWindowPos(
				windowHandle,
				nullptr,
				position.first,
				position.second,
				size.first,
				size.second,
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_FRAMECHANGED
			);
		}
	}
}