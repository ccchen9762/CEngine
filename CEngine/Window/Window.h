#pragma once

#include "InitWindows.h"
#include <string>

#include "CEngine/Exception/BaseException.h"
#include "CEngine/Input/Keyboard.h"
#include "CEngine/Input/Mouse.h"
#include "CEngine/Graphics/Graphics.h"

class Window {
private:
    // singleton for window class registration
    class WindowClass {
    public:
        WindowClass();
        ~WindowClass();
        // not copyable
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator= (const WindowClass&) = delete;

        static HINSTANCE getHInstance() { return m_WndClass.m_hInstance; }
        static const TCHAR* getWndClassName() { return m_WndClassName; }

    private:
        static WindowClass m_WndClass;
        HINSTANCE m_hInstance;
        static const TCHAR* m_WndClassName;
    };
public:
    class WindowException : public BaseException {
    public:
        WindowException(const char* file, int line, HRESULT hResult);

        const char* what() const noexcept override;
        virtual const char* getType() const { return "WindowException"; }
        virtual const wchar_t* getWType() const { return L"WindowException"; }
        HRESULT getHResult() { return m_hResult; }
        std::string getErrorString() const;

    private:
        HRESULT m_hResult;
    };

public:
    Window(const TCHAR* title, int width, int height);
    ~Window();
    // not copyable
    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;

    void setTitle(const std::wstring& title);
    Graphics* getGraphics() { return pGraphics; }

    void CaptureWindow() { SetCapture(m_hWnd); }

    Keyboard m_keyboard;
    Mouse m_mouse;

private:
    static LRESULT WINAPI WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT WINAPI WndProcRelay(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    Graphics* pGraphics;

    HWND m_hWnd;
    int m_width;
    int m_height;
};

#define WINDOW_EXCEPT(hResult) Window::WindowException(__FILE__, __LINE__, hResult)
#define WINDOW_LAST_EXCEPT() Window::WindowException(__FILE__, __LINE__, GetLastError())