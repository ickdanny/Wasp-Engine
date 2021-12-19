#include "MainWindow.h"

namespace windowwrapper {
	LRESULT MainWindow::handleMessage(UINT messageCode, WPARAM wParam, LPARAM lParam) {
		switch (messageCode)
		{
			case WM_CREATE:
				if (FAILED(windowPainter.init())){
					return -1;  // Fail CreateWindowEx.
				}
				return 0;

			case WM_PAINT:
				windowPainter.paint(windowHandle);
				return 0;

			case WM_SIZE:
				windowPainter.resize(windowHandle);
				return 0;
		
			case WM_DESTROY:
				windowPainter.cleanUp();
				PostQuitMessage(0);
				return 0;

			default:
				return DefWindowProc(windowHandle, messageCode, wParam, lParam);
		}
		return TRUE;
	}
}