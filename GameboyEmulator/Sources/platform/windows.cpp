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
    , m_Pixels(nullptr)
{

}

Window::~Window()
{
    Destroy();
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
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        winParam->hInstance, NULL)) == NULL)
    {
        return false;
    }

    RECT windowRect;
    SetRect(&windowRect, 0, 0, winParam->Width * winParam->Scale, winParam->Height * winParam->Scale);
    AdjustWindowRectEx(&windowRect, GetWindowLong(m_hWnd, GWL_STYLE), GetMenu(m_hWnd) != NULL, GetWindowLong(m_hWnd, GWL_EXSTYLE));
    const int nWidth = windowRect.right - windowRect.left;
    const int nHeight = windowRect.bottom - windowRect.top;
    SetWindowPos(m_hWnd, NULL, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

    HDC hdc = GetDC(m_hWnd);
    m_hBitmap = CreateCompatibleBitmap(hdc, winParam->Width, winParam->Height);
    m_hScreenDC = CreateCompatibleDC(hdc);
    SelectObject(m_hScreenDC, m_hBitmap);
    ReleaseDC(m_hWnd, hdc);

    m_Pixels = (LPDWORD)HeapAlloc(GetProcessHeap(), 
        HEAP_ZERO_MEMORY, static_cast<uint64_t>(m_ClientWidth) * m_ClientHeight * 4);
    
    return m_hWnd != NULL;
}

void Window::Run()
{
    MSG message;

    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        //TranslateMessage(&message);
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
    if (m_Pixels)
    {
        HeapFree(GetProcessHeap(), 0, m_Pixels);
        m_Pixels = nullptr;
    }
}

void Window::DrawBegin()
{
    m_NowTime = clock();
}

void Window::DrawPixel(const int32_t x, const int32_t y, const uint32_t color)
{
#if false
    const byte [[maybe_unused]] a = (color >> 24) & 0xff;
    const byte r = (color >> 16) & 0xff;
    const byte g = (color >>  8) & 0xff;
    const byte b = (color >>  0) & 0xff;
    SetPixelV(m_hScreenDC, x, y, RGB(r, g, b));
#else
    m_Pixels[x + ((m_ClientHeight - 1) - y) * m_ClientWidth] = color;
#endif
}

void Window::DrawEnd()
{
    clock_t progress = clock() - m_NowTime;

    BITMAPINFO bmpInfo;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = m_ClientWidth;
    bmpInfo.bmiHeader.biHeight = m_ClientHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    GetClientRect(m_hWnd, &rect);
    SetStretchBltMode(hdc, COLORONCOLOR);
    StretchDIBits(
        hdc, 0, 2, rect.right, rect.bottom, 0, 0,
        m_ClientWidth, m_ClientHeight,
        m_Pixels, &bmpInfo, DIB_RGB_COLORS, SRCCOPY
    );
    EndPaint(m_hWnd, &ps);

#if false
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(m_hWnd, &ps);
    BitBlt(hdc, 0, 0, m_ClientWidth, m_ClientHeight, m_hScreenDC, 0, 0, SRCCOPY);
    PatBlt(m_hScreenDC, 0, 0, m_ClientWidth, m_ClientHeight, BLACKNESS);
    EndPaint(m_hWnd, &ps);
#endif

    InvalidateRect(m_hWnd, NULL, FALSE);
}

#endif // _WIN64
