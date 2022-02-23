#pragma once
#if _WIN64

#include <windows.h>
#include <ctime>
#include <gbl/emulate/lcd.h>
#include "window.h"

class Window : public IWindow, public gbl::ILCD
{
public:
    struct CreateParameter : public IWindow::CreateParameter
    {
        int Width;
        int Height;
        HINSTANCE hInstance;
    };

public:
    Window();
    ~Window();

    bool Create(const IWindow::CreateParameter* param) override;
    void Run() override;
    void Show();
    void Destroy() override;

    void DrawBegin() override;
    void DrawPixel(int32_t x, int32_t y, uint32_t color) override;
    void DrawEnd() override;

private:
    HWND m_hWnd;
    HBITMAP m_hBitmap;
    HDC m_hScreenDC;
    int32_t m_ClientWidth;
    int32_t m_ClientHeight;
    clock_t m_NowTime;
};
#endif // _WIN64
