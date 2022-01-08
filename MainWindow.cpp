#include "MainWindow.h"

namespace wasp::windowadapter {

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
		
			case WM_DESTROY:
				quitGameCallback();
				PostQuitMessage(0);
				return 0;

			default:
				return DefWindowProc(windowHandle, messageCode, wParam, lParam);
		}
		return TRUE;
	}
}