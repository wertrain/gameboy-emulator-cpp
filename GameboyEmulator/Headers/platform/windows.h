#pragma once
#if _WIN64

#include <windows.h>
#include <ctime>
#include <gbl/core/ppu.h>
#include <gbl/emulate/lcd.h>
#include "window.h"

class Window : public IWindow, public gbl::ILCD
{
public:
    struct CreateParameter : public IWindow::CreateParameter
    {
        int Width;
        int Height;
        int Scale;
        HINSTANCE hInstance;

        CreateParameter()
            : Width(gbl::PPU::BUFFER_WIDTH)
            , Height(gbl::PPU::BUFFER_HEIGHT)
            , Scale(3)
            , hInstance(NULL) {}
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
    LPDWORD m_Pixels;
};
#endif // _WIN64
