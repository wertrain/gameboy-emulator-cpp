#include <memory>
#include <gbl/gbl.h>

#if _WIN64

#include "platform/windows.h"

namespace
{

static DWORD s_dwThreadId;

DWORD WINAPI EmulatorThread(LPVOID vdParam)
{
    auto* gb = static_cast<gbl::GameBoyEmulator*>(vdParam);
    gb->Boot(R"(D:\Develop\C++\sample\mem_timing.gb)");
    gb->Shutdown();
    return TRUE;
}

}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, 
    _In_ PSTR lpCmdLine, _In_ int nCmdShow)
{
    std::unique_ptr<Window> window(new Window);

    Window::CreateParameter param;
    param.hInstance = hInstance;
    window->Create(&param);
    window->Show();

    std::unique_ptr<gbl::GameBoyEmulator> gb(new gbl::GameBoyEmulator());
    gb->SetLCD(window.get());

    auto hThread = CreateThread(NULL,
        0,
        EmulatorThread,
        (LPVOID)gb.get(),
        0,
        &s_dwThreadId);

    if (hThread != NULL)
    {
        window->Run();
        gb->StopRunning();

        DWORD dwParam;
        GetExitCodeThread(hThread, &dwParam);
        WaitForSingleObject(hThread, INFINITE);
    }

    return 0;
}

#else

int main()
{
    return 0;
}

#endif // _WIN64
