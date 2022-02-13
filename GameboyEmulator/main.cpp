#include <gbl/gbl.h>

int main()
{
    auto* gb = new gbl::GameBoyEmulator();
    gb->Boot(R"(sample.rom)");
    gb->Shutdown();

    return 0;
}
