#include "MainWindow.h"

namespace windowwrapper {
    LRESULT MainWindow::handleMessage(UINT messageCode, WPARAM wParam, LPARAM lParam) {
        switch (messageCode)
        {
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(windowHandle, &ps);
                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
                EndPaint(windowHandle, &ps);
            }
            return 0;

            default:
                return DefWindowProc(windowHandle, messageCode, wParam, lParam);
        }
        return TRUE;
    }
}