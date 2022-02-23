#if _WIN64

#include "platform/windows.h"

namespace
{

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        return 0;

    case WM_PAINT:
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

}

Window::Window()
    : m_hWnd()
    , m_hBitmap()
    , m_hScreenDC()
    , m_ClientWidth(0)
    , m_ClientHeight(0)
    , m_NowTime(0)
{

}

Window::~Window()
{

}

bool Window::Create(const IWindow::CreateParameter* param)
{
    const auto* winParam = static_cast<const CreateParameter*>(param);
    m_ClientWidth = winParam->Width;
    m_ClientHeight = winParam->Height;

    WNDCLASSEX winc;
    ZeroMemory(&winc, sizeof(WNDCLASSEX));
    winc.cbSize = sizeof(WNDCLASSEX);
    winc.style = CS_HREDRAW | CS_VREDRAW;
    winc.lpfnWndProc = ::WndProc;
    winc.cbClsExtra = winc.cbWndExtra = 0;
    winc.hInstance = winParam->hInstance;
    winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    winc.lpszClassName = TEXT("GameboyEmulator");

    if (!RegisterClassEx(&winc))
    {
        return false;
    }

    if ((m_hWnd = CreateWindowEx(
        0, TEXT("GameboyEmulator"), TEXT("Gameboy Emulator"),
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        winParam->hInstance, NULL)) == NULL)
    {
        return false;
    }

    RECT windowRect;
    SetRect(&windowRect, 0, 0, winParam->Width, winParam->Height);
    AdjustWindowRectEx(&windowRect, GetWindowLong(m_hWnd, GWL_STYLE), GetMenu(m_hWnd) != NULL, GetWindowLong(m_hWnd, GWL_EXSTYLE));
    const int nWidth = windowRect.right - windowRect.left;
    const int nHeight = windowRect.bottom - windowRect.top;
    SetWindowPos(m_hWnd, NULL, 162, 162, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

    HDC hdc = GetDC(m_hWnd);
    m_hBitmap = CreateCompatibleBitmap(hdc, winParam->Width, winParam->Height);
    m_hScreenDC = CreateCompatibleDC(hdc);
    SelectObject(m_hScreenDC, m_hBitmap);
    ReleaseDC(m_hWnd, hdc);
    
    return m_hWnd != NULL;
}

void Window::Run()
{
    MSG message;

    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        DispatchMessage(&message);
    }
}

void Window::Show()
{
    if (IsWindow(m_hWnd))
    {
        ShowWindow(m_hWnd, SW_SHOWDEFAULT);
        UpdateWindow(m_hWnd);
    }
}

void Window::Destroy()
{

}

void Window::DrawBegin()
{
    m_NowTime = clock();
}

void Window::DrawPixel(const int32_t x, const int32_t y, const uint32_t color)
{
    const byte [[maybe_unused]] a = (color >> 24) & 0xff;
    const byte r = (color >> 16) & 0xff;
    const byte g = (color >>  8) & 0xff;
    const byte b = (color >>  0) & 0xff;

    SetPixel(m_hScreenDC, x, y, RGB(r, g, b));
}

void Window::DrawEnd()
{
    clock_t progress = clock() - m_NowTime;

    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(m_hWnd, &ps);
    BitBlt(hdc, 0, 0, m_ClientWidth, m_ClientHeight, m_hScreenDC, 0, 0, SRCCOPY);
    PatBlt(m_hScreenDC, 0, 0, m_ClientWidth, m_ClientHeight, BLACKNESS);
    EndPaint(m_hWnd, &ps);

    InvalidateRect(m_hWnd, NULL, FALSE);
}

#endif // _WIN64
