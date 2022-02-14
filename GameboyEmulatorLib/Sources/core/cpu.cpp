#include "pch.h"
#include "framework.h"

#include <memory>
#include "gbl/allocator.h"
#include "gbl/core/mmu.h"
#include "gbl/core/cpuop/cpuop.h"
#include "gbl/core/cpu.h"

namespace gbl
{

CPU::CPU(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_CPU(nullptr)
    , m_Operator(nullptr)
{
    m_CPU = static_cast<CPU_t*>(m_Allocator->Allocate(sizeof(CPU_t)));
    memset(m_CPU, 0, sizeof(CPU_t));

    m_Operator = gbl::New<CPUOperator>(m_Allocator);
}

CPU::~CPU()
{
    gbl::Delete<CPUOperator>(m_Operator, m_Allocator);

    if (m_CPU)
    {
        free(m_CPU);
        m_CPU = nullptr;
    }
}

void CPU::Tick(MMU* mmu)
{
    // ñΩóﬂÇÃì«Ç›éÊÇË
    uint8_t op = mmu->ReadByte(m_CPU->registers.pc.word);
    m_CPU->currentOp = op << 8;
    m_CPU->currentOpAddress = m_CPU->registers.pc.word;

    // ñΩóﬂÇÃé¿çs

}

void CPU::Reset()
{
    memset(reinterpret_cast<void*>(&m_CPU->registers), 0, sizeof(m_CPU->registers));
    m_CPU->ime = false;
    m_CPU->currentClock = { 0, 0 };
    m_CPU->totalClock = { 0, 0 };
}

void CPU::InstructionInc8(uint8_t* reg)
{

}

} // namespace gbl
