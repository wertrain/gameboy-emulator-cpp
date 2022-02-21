#include <gbl/gbl.h>
#include <iostream>

class LCD : public gbl::ILCD
{
public:
    void DrawBegin() override
    {
        for (int y = 0; y < gbl::LcdHeight; ++y)
        {
            for (int x = 0; x < gbl::LcdWidth; ++x)
            {
                m_Display[y][x] = 0;
            }
        }
    }

    void DrawPixel(int32_t x, int32_t y, uint32_t color) override
    {
        m_Display[y][x] = color;
        if (color != 0)
        {
            m_Display[y][x] = 1;
        }
    }

    void DrawEnd() override
    {
        for (int y = 0; y < gbl::LcdHeight; ++y)
        {
            for (int x = 0; x < gbl::LcdWidth; ++x)
            {
                std::cout << m_Display[y][x];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

private:
    uint32_t m_Display[gbl::LcdHeight][gbl::LcdWidth];
};

int main()
{
    std::unique_ptr<LCD> lcd(new LCD());
    std::unique_ptr<gbl::GameBoyEmulator> gb(new gbl::GameBoyEmulator());

    gb->SetLCD(lcd.get());
    gb->Boot(R"(D:\Develop\C++\sample\mem_timing.gb)");
    gb->Shutdown();

    return 0;
}
