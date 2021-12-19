#pragma once

#include "framework.h" //includes window.h and others

namespace windowwrapper {
    template<class DERIVED_CLASS>
    class BaseWindow {

    protected:
        //set in WindowProc //pThis->windowHandle = windowHandle;
        HWND windowHandle;

    public:
        static LRESULT CALLBACK WindowProc(HWND windowHandle, UINT messageCode, WPARAM wParam, LPARAM lParam)
        {
            DERIVED_CLASS* pThis = NULL;

            if (messageCode == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                pThis = (DERIVED_CLASS*)pCreate->lpCreateParams;
                SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)pThis);

                pThis->windowHandle = windowHandle;
            }
            else
            {
                pThis = (DERIVED_CLASS*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
            }
            if (pThis)
            {
                return pThis->handleMessage(messageCode, wParam, lParam);
            }
            else
            {
                return DefWindowProc(windowHandle, messageCode, wParam, lParam);
            }
        }

        BaseWindow() : windowHandle{ NULL } { }

        bool create(
            HINSTANCE instanceHandle,
            PCWSTR className,
            PCWSTR windowName,
            DWORD windowStyle,
            DWORD extraWindowStyle = 0,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            int width = CW_USEDEFAULT,
            int height = CW_USEDEFAULT,
            HWND parentWindowHandle = 0,
            HMENU menuHandle = 0
        ) {
            registerWindowClass(instanceHandle, className);

            return createWindow(
                instanceHandle,
                className,
                windowName,
                windowStyle,
                extraWindowStyle,
                x,
                y,
                width,
                height,
                parentWindowHandle,
                menuHandle
            );
        }

        HWND getWindowHandle() const { return windowHandle; }

    protected:
        virtual LRESULT handleMessage(UINT messageCode, WPARAM wParam, LPARAM lParam) = 0;

    private:
        void registerWindowClass(HINSTANCE instanceHandle, const wchar_t* className) {

            WNDCLASS wc{ };

            wc.lpfnWndProc = DERIVED_CLASS::WindowProc;
            wc.hInstance = instanceHandle;
            wc.lpszClassName = className;

            RegisterClass(&wc);
        }

        //returns false if failed
        bool createWindow(
            HINSTANCE instanceHandle,
            PCWSTR className,
            PCWSTR windowName,
            DWORD windowStyle,
            DWORD extraWindowStyle,
            int x,
            int y,
            int width,
            int height,
            HWND parentWindowHandle,
            HMENU menuHandle
        ) {

            HWND windowHandle = CreateWindowEx(
                extraWindowStyle,       // Optional window styles.
                className,              // Window class
                windowName,             // Window text
                windowStyle,            // Window style
                x, y, width, height,    // Size and position
                parentWindowHandle,     // Parent window    
                menuHandle,             // Menu
                instanceHandle,         // Instance handle
                this                    // Additional application data
            );

            return windowHandle != NULL;
        }
    };
}