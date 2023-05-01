#include "Window.h"

#include <sstream>

Window::WindowClass Window::WindowClass::m_WndClass;
const TCHAR* Window::WindowClass::m_WndClassName = L"D3D11_Wnd_Class";

Window::WindowClass::WindowClass() : m_hInstance(GetModuleHandle(nullptr)) {
    // register window class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WndProcSetup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = m_WndClassName;
    wcex.hIconSm = nullptr;
    RegisterClassEx(&wcex);
}

Window::WindowClass::~WindowClass() {
    UnregisterClass(m_WndClassName, getHInstance());
}


Window::Window(const TCHAR* title, int width, int height) : 
    m_width(width), m_height(height), pGraphics(nullptr) {

    // adjust size for exact client space, add 100 for shrinking
    RECT clientSpace;
    clientSpace.left = 100;
    clientSpace.right = width + 100;
    clientSpace.top = 100;
    clientSpace.bottom = height + 100;
    AdjustWindowRect(&clientSpace, WS_OVERLAPPEDWINDOW, false);

    // create and show window 
    m_hWnd = CreateWindowEx(0, WindowClass::getWndClassName(), title, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT,
                            clientSpace.right - clientSpace.left, clientSpace.bottom - clientSpace.top,
                            nullptr, nullptr, WindowClass::getHInstance(), this);

    SetCapture(m_hWnd);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    pGraphics = new Graphics(m_hWnd, width, height);
}

Window::~Window() {
    delete pGraphics;
    DestroyWindow(m_hWnd);
}

void Window::setTitle(const std::wstring& title) {
    SetWindowText(m_hWnd, title.c_str());
}

// binding message handle to pointer to Window instance
LRESULT WINAPI Window::WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // event just before the window creation
    if (msg == WM_NCCREATE) {
        // extract pointer to window class
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        // store pointer to GWLP_USERDATA
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        // switch message handle function to WndProcRelay
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WndProcRelay));
        // let WndProc handle the rest
        return pWindow->WndProc(hWnd, msg, wParam, lParam);
    }
    // return default until window creation
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::WndProcRelay(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // get pointer to Window class instance
    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWindow->WndProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KILLFOCUS:{
        m_keyboard.ClearKeyState();
        break;
    }
    // mouse handling
    case WM_MOUSEMOVE: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        if (mousePos.x >= 0 && mousePos.x < m_width && mousePos.y >= 0 && mousePos.y < m_height) {
            m_mouse.OnMouseMove(Vector3Int(mousePos.x, mousePos.y));
            if (!m_mouse.IsInWindow()) {
                SetCapture(hWnd);
                m_mouse.OnMouseEnter();
            }
        }
        else {
            if (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
                m_mouse.OnMouseMove(Vector3Int(mousePos.x, mousePos.y));
            }
            else {
                ReleaseCapture();
                m_mouse.OnMouseLeave();
            }
        }
        break;
    } 
    case WM_LBUTTONDOWN: {
        SetForegroundWindow(hWnd);
        SetCapture(hWnd);
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnLButtonPressed(Vector3Int(mousePos.x, mousePos.y));
        break;
    }
    case WM_MBUTTONDOWN: {
        SetForegroundWindow(hWnd);
        SetCapture(hWnd);
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnMButtonPressed(Vector3Int(mousePos.x, mousePos.y));
        break;
    }
    case WM_RBUTTONDOWN: {
        SetForegroundWindow(hWnd);
        SetCapture(hWnd);
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnRButtonPressed(Vector3Int(mousePos.x, mousePos.y));
        break;
    }
    case WM_LBUTTONUP: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnLButtonReleased(Vector3Int(mousePos.x, mousePos.y));
        if (mousePos.x < 0 || mousePos.x >= m_width || mousePos.y < 0 || mousePos.y >= m_height) {
            ReleaseCapture();
            m_mouse.OnMouseLeave();
        }
        break;
    }
    case WM_MBUTTONUP: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnMButtonReleased(Vector3Int(mousePos.x, mousePos.y));
        if (mousePos.x < 0 || mousePos.x >= m_width || mousePos.y < 0 || mousePos.y >= m_height) {
            ReleaseCapture();
            m_mouse.OnMouseLeave();
        }
        break;
    }
    case WM_RBUTTONUP: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        m_mouse.OnRButtonReleased(Vector3Int(mousePos.x, mousePos.y));
        if (mousePos.x < 0 || mousePos.x >= m_width || mousePos.y < 0 || mousePos.y >= m_height) {
            ReleaseCapture();
            m_mouse.OnMouseLeave();
        }
        break;
    }
    case WM_MOUSEWHEEL: {
        const POINTS mousePos = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        m_mouse.OnWheelScrolled(Vector3Int(mousePos.x, mousePos.y), delta);
        break;
    }
    // Keyboard handling 
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (m_keyboard.IsAutoRepeatEnabled() || !(lParam & 0x40000000)) {
            m_keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        m_keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        m_keyboard.OnCharTyped(static_cast<unsigned char>(wParam));
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::WindowException::WindowException(const char* file, int line, HRESULT hResult) :
    BaseException(file, line), m_hResult(hResult){
}

const char* Window::WindowException::what() const {
    std::ostringstream oss;
    oss << getType() << "\n" <<
        "Error Code: " << m_hResult << "\n" <<
        "Description: " << getErrorString() << "\n" <<
        getErrorLocation();

    m_whatString = oss.str();

    return m_whatString.c_str();
}

std::string Window::WindowException::getErrorString() const{
    wchar_t* pMsg;

    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, m_hResult, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        reinterpret_cast<LPWSTR>(&pMsg), 0, nullptr
    );  // need to specify using english
    
    if (nMsgLen == 0)
        return "Unidentified error code";

    m_whatWString = pMsg;
    std::string errorString(m_whatWString.begin(), m_whatWString.end());
    LocalFree(pMsg);

    return errorString;
}
