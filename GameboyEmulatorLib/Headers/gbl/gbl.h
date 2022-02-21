#pragma once

#include "gbl/core/ppu.h"
#include "gbl/emulate/cartridge.h"
#include "gbl/emulate/lcd.h"
#include "gbl/emulate/gameboy.h"

namespace gbl
{

/// <summary>
/// LCD screen width
/// </summary>
constexpr uint32_t LcdWidth = PPU::BUFFER_WIDTH;

/// <summary>
/// LCD screen height
/// </summary>
constexpr uint32_t LcdHeight = PPU::BUFFER_HEIGHT;

}
