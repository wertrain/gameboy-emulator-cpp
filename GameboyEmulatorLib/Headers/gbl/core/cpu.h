#pragma once

#include <cinttypes>

namespace gbl { class IGBLAllocator; }
namespace gbl { class MMU; }
namespace gbl { class CPUOperator; }

namespace gbl
{

class CPU
{
public:
    CPU(IGBLAllocator* allocator);
    ~CPU();

    void Tick(MMU* mmu);
    void Reset();

private:
    typedef union Register16Type
    {
        struct
        {
            uint8_t lo;
            uint8_t hi;
        };
        uint16_t word;
    }
    Reg16_t;

    typedef struct ClockType
    {
        uint32_t m;
        uint32_t t;
    }
    Clock_t;

    typedef struct CPUType
    {
        Clock_t currentClock;
        Clock_t totalClock;

        struct
        {
            Reg16_t af, bc, de, hl;
            Reg16_t sp, pc;
        }
        registers;

        bool ime;

        uint16_t currentOp;
        uint16_t currentOpAddress;
    }
    CPU_t;

    typedef struct OpFuncType
    {
        void (*function)(MMU* mmu);
        uint8_t m;
        uint8_t t;
        uint8_t b;
    }
    OpFunc_t;

private:
    IGBLAllocator* m_Allocator;
    CPU_t* m_CPU;
    CPUOperator* m_Operator;

    friend class CPUOperator;
};

} // namespace gbl
