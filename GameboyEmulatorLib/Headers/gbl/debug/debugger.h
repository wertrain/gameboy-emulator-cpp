#pragma once

namespace gbl { class CPU; }

namespace gbl
{

class Debugger
{
public:
    Debugger();
    ~Debugger();

public:
    static void OpcodeError(CPU* cpu);
};

} // namespace gbl
