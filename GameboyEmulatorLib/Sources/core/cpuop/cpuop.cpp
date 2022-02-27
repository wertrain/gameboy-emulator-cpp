#include "pch.h"
#include "framework.h"

#include "gbl/core/mmu.h"
#include "gbl/core/cpu.h"
#include "gbl/core/cpuop/cpuop.h"

namespace gbl
{

CPUOperator::CPUOperator(IGBLAllocator* allocator)
    : m_Allocator(allocator)
    , m_OpTable()
    , m_OpCbTable()
{

}

CPUOperator::~CPUOperator()
{

}

void CPUOperator::Initialize()
{
#define OPCODE(address, mnemonic, length, cycles0, cycles1, group, flagZ, flagN, flagH, flagC, operand1, operand2, count) m_OpTable[address] = { &CPUOperator::##Op_##address, length, cycles0, count };
#include "gbl/core/cpuop/cpuop_table.hxx"
#undef OPCODE

    // Op_cb ‚Ì‚Ý“ÁŽê

#define OPCODE(address, mnemonic, length, cycles0, cycles1, group, flagZ, flagN, flagH, flagC, operand1, operand2, count) m_OpCbTable[address] = { &CPUOperator::##OpCb_##address, length, cycles0, count };
#include "gbl/core/cpuop/cpuop_cb_table.hxx"
#undef OPCODE
}

CPUOperator::OpFunc_t* CPUOperator::GetOperator(const uint8_t index)
{
    return &m_OpTable[index];
}

CPUOperator::OpFunc_t* CPUOperator::GetCbOperator(const uint8_t index)
{
    return &m_OpCbTable[index];
}

void CPUOperator::Op_0x00(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "NOP");
}

void CPUOperator::Op_0x01(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD BC, $%04x", word);
    cpu->m_CPU->registers.bc.word = word;
}

void CPUOperator::Op_0x02(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (BC), A");
    mmu->WriteByte(cpu->m_CPU->registers.bc.word, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x03(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC BC");
    cpu->m_CPU->registers.bc.word++;
}

void CPUOperator::Op_0x04(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC B");
    cpu->InstructionInc8(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0x05(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC B");
    cpu->InstructionDec8(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0x0c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC C");
    cpu->InstructionInc8(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0x06(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD B, $%02X", byte);
    cpu->m_CPU->registers.bc.hi = byte;
}

void CPUOperator::Op_0x07(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLCA");
    cpu->InstructionRlc(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_0x08(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($%02X), SP");
    mmu->WriteWord(addr, cpu->m_CPU->registers.sp.word);
}

void CPUOperator::Op_0x09(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, BC");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.word, false);
}

void CPUOperator::Op_0x0b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC BC");
    cpu->m_CPU->registers.bc.word--;
}

void CPUOperator::Op_0x0a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (BC)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.bc.word);
}

void CPUOperator::Op_0x0d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC C");
    cpu->InstructionDec8(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0x0e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD C, $%02X", byte);
    cpu->m_CPU->registers.bc.lo = byte;
}

void CPUOperator::Op_0x0f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRCA");
    cpu->InstructionRrc(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_0x11(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD DE, $%04x", word);
    cpu->m_CPU->registers.de.word = word;
}

void CPUOperator::Op_0x12(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (DE), A");
    mmu->WriteByte(cpu->m_CPU->registers.de.word, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x13(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC DE");
    cpu->m_CPU->registers.de.word++;
}

void CPUOperator::Op_0x14(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC D");
    cpu->InstructionInc8(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0x15(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC D");
    cpu->InstructionDec8(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0x16(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD D, $%02X", byte);
    cpu->m_CPU->registers.de.hi = byte;
}

void CPUOperator::Op_0x17(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLA");
    cpu->InstructionRl(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_0x18(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR $%02x", (uint8_t)offset);
    cpu->m_CPU->registers.pc.word += offset;
}

void CPUOperator::Op_0x19(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, DE");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.word, false);
}

void CPUOperator::Op_0x1a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (DE)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.de.word);
}

void CPUOperator::Op_0x1b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC DE");
    cpu->m_CPU->registers.de.word--;
}

void CPUOperator::Op_0x1c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC E");
    cpu->InstructionInc8(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0x1d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC E");
    cpu->InstructionDec8(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0x1e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD E, $%02X", byte);
    cpu->m_CPU->registers.de.lo = byte;
}

void CPUOperator::Op_0x1f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRA");
    cpu->InstructionRr(&cpu->m_CPU->registers.af.hi);
    cpu->FlagSetZero(false);
}

void CPUOperator::Op_0x20(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR NZ, $%2X", (uint8_t)offset);
    cpu->InstructionJr(offset, CPU::Condition::NZ);
}

void CPUOperator::Op_0x21(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD HL, $%04X", word);
    cpu->m_CPU->registers.hl.word = word;
}

void CPUOperator::Op_0x22(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL+), A");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
    cpu->m_CPU->registers.hl.word++;
}

void CPUOperator::Op_0x23(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC HL");
    cpu->m_CPU->registers.hl.word++;
}

void CPUOperator::Op_0x24(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC H");
    cpu->InstructionInc8(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0x25(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC H");
    cpu->InstructionDec8(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0x26(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD H, $%02X", byte);
    cpu->m_CPU->registers.hl.hi = byte;
}

void CPUOperator::Op_0x27(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DAA");
    uint8_t a = cpu->m_CPU->registers.af.hi;

    if (cpu->Flag(CPU::FLAG_SUB_BIT))
    {
        if (cpu->Flag(CPU::FLAG_HC_BIT))
        {
            a = (a - 0x06) & 0xFF;
        }

        if (cpu->Flag(CPU::FLAG_CARRY_BIT))
        {
            a -= 0x60;
        }
    }
    else
    {
        if (cpu->Flag(CPU::FLAG_HC_BIT) || (a & 0xF) > 9)
        {
            a += 0x06;
        }

        if (cpu->Flag(CPU::FLAG_CARRY_BIT) || a > 0x9F)
        {
            a += 0x60;
        }
    }

    cpu->m_CPU->registers.af.hi = a;

    cpu->FlagSetHalfcarry(false);
    cpu->FlagSetZero(a == 0);
    cpu->FlagSetCarry(a >= 0x100);
}

void CPUOperator::Op_0x28(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR Z, $%2X", (uint8_t)offset);
    cpu->InstructionJr(offset, CPU::Condition::Z);
}

void CPUOperator::Op_0x29(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, HL");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.word, false);
}

void CPUOperator::Op_0x2a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL+)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.word++;
}

void CPUOperator::Op_0x2b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC HL");
    cpu->m_CPU->registers.hl.word--;
}

void CPUOperator::Op_0x2c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC L");
    cpu->InstructionInc8(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0x2d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC L");
    cpu->InstructionDec8(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0x2e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD L, $%02X", byte);
    cpu->m_CPU->registers.hl.lo = byte;
}

void CPUOperator::Op_0x2f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CPL");
    cpu->FlagSetSub(false);
    cpu->FlagSetHalfcarry(false);
    cpu->m_CPU->registers.af.hi = ~cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x30(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR NC, $%2X", (uint8_t)offset);
    cpu->InstructionJr(offset, CPU::Condition::NC);
}

void CPUOperator::Op_0x31(CPU* cpu, MMU* mmu)
{
    uint16_t word = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD SP, $%04X", word);

    cpu->m_CPU->registers.sp.word = word;
}

void CPUOperator::Op_0x32(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL-), A");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
    cpu->m_CPU->registers.hl.word--;
}

void CPUOperator::Op_0x33(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC SP");
    cpu->m_CPU->registers.sp.word++;
}

void CPUOperator::Op_0x34(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionInc8(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_0x35(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionDec8(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::Op_0x36(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD (HL), $%02X", byte);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, byte);
}

void CPUOperator::Op_0x37(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SCF");
    cpu->FlagSetSub(false);
    cpu->FlagSetHalfcarry(false);
    cpu->FlagSetCarry(true);
}

void CPUOperator::Op_0x38(CPU* cpu, MMU* mmu)
{
    int8_t offset = static_cast<int8_t>(mmu->ReadByte(cpu->m_CPU->registers.pc.word));
    DebugInstruction(cpu, mmu, "JR C, $%2X", (uint8_t)offset);
    cpu->InstructionJr(offset, CPU::Condition::C);
}

void CPUOperator::Op_0x39(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD HL, SP");
    cpu->InstructionAdd16(&cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.sp.word, false);
}

void CPUOperator::Op_0x3a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL-)");
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.word--;
}

void CPUOperator::Op_0x3b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC SP");
    cpu->m_CPU->registers.sp.word--;
}

void CPUOperator::Op_0x3c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "INC A");
    cpu->InstructionInc8(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x3d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DEC A");
    cpu->InstructionDec8(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x3e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, $%02X", byte);
    cpu->m_CPU->registers.af.hi = byte;
}

void CPUOperator::Op_0x3f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CCF");
    cpu->FlagSetSub(false);
    cpu->FlagSetHalfcarry(false);
    cpu->FlagSetCarry(!cpu->Flag(CPU::FLAG_CARRY_BIT));
}


void CPUOperator::Op_0x40(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, B");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_0x41(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, C");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_0x42(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, D");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_0x43(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, E");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_0x44(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, H");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_0x45(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, L");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_0x46(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.bc.hi = byte;
}

void CPUOperator::Op_0x47(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD B, A");
    cpu->m_CPU->registers.bc.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x48(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, B");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_0x49(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, C");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_0x4a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, D");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_0x4b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, E");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_0x4c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, H");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_0x4d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, L");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_0x4e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.bc.lo = byte;
}

void CPUOperator::Op_0x4f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD C, A");
    cpu->m_CPU->registers.bc.lo = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x50(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, B");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_0x51(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, C");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_0x52(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, D");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_0x53(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, E");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_0x54(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, H");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_0x55(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, L");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_0x56(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.de.hi = byte;
}

void CPUOperator::Op_0x57(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD D, A");
    cpu->m_CPU->registers.de.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x58(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, B");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_0x59(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, C");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_0x5a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, D");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_0x5b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, E");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_0x5c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, H");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_0x5d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, L");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_0x5e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.de.lo = byte;
}

void CPUOperator::Op_0x5f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD E, A");
    cpu->m_CPU->registers.de.lo = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x60(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, B");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_0x61(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, C");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_0x62(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, D");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_0x63(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, E");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_0x64(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, H");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_0x65(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, L");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_0x66(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.hi = byte;
}

void CPUOperator::Op_0x67(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD H, A");
    cpu->m_CPU->registers.hl.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x68(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, B");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_0x69(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, C");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_0x6a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, D");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_0x6b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, E");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_0x6c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, H");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_0x6d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, L");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_0x6e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.hl.lo = byte;
}

void CPUOperator::Op_0x6f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD L, A");
    cpu->m_CPU->registers.hl.lo = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x70(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), B");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0x71(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), C");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0x72(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), D");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0x73(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), E");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0x74(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), H");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0x75(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), L");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0x76(CPU* cpu, MMU* mmu)
{
    // NOP
}

void CPUOperator::Op_0x77(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD (HL), A");
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x78(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, B");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.bc.hi;
}

void CPUOperator::Op_0x79(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, C");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.bc.lo;
}

void CPUOperator::Op_0x7a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, D");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.de.hi;
}

void CPUOperator::Op_0x7b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, E");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.de.lo;
}

void CPUOperator::Op_0x7c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, H");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.hl.hi;
}

void CPUOperator::Op_0x7d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, L");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.hl.lo;
}

void CPUOperator::Op_0x7e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, (HL)");
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->m_CPU->registers.af.hi = byte;
}

void CPUOperator::Op_0x7f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, A");
    cpu->m_CPU->registers.af.hi = cpu->m_CPU->registers.af.hi;
}

void CPUOperator::Op_0x80(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, B");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0x81(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, C");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0x82(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, D");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0x83(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, E");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0x84(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, H");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0x85(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, L");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0x86(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "ADD A, (HL)");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, value);
}

void CPUOperator::Op_0x87(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADD A, A");
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x88(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, B");
    cpu->InstructionAdc(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0x89(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, C");
    cpu->InstructionAdc(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0x8a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, D");
    cpu->InstructionAdc(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0x8b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, E");
    cpu->InstructionAdc(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0x8c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, H");
    cpu->InstructionAdc(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0x8d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, L");
    cpu->InstructionAdc(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0x8e(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "ADC A, (HL)");
    cpu->InstructionAdc(byte);
}

void CPUOperator::Op_0x8f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "ADC A, A");
    cpu->InstructionAdc(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x90(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB B");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0x91(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB C");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0x92(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB D");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0x93(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB E");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0x94(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB H");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0x95(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB L");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0x96(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB (HL)");
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSub8(value);
}

void CPUOperator::Op_0x97(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SUB A");
    cpu->InstructionSub8(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0x98(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, B");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0x99(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, C");
    cpu->InstructionSub8(cpu->m_CPU->registers.bc.lo + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0x9a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, D");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0x9b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, E");
    cpu->InstructionSub8(cpu->m_CPU->registers.de.lo + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0x9c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, H");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0x9d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, L");
    cpu->InstructionSub8(cpu->m_CPU->registers.hl.lo + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0x9e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, (HL)");
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSub8(value + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0x9f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SBC A, A");
    cpu->InstructionSub8(cpu->m_CPU->registers.af.hi + cpu->Flag(CPU::FLAG_CARRY_BIT));
}

void CPUOperator::Op_0xa0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND B");
    cpu->InstructionAnd(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0xa1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND C");
    cpu->InstructionAnd(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0xa2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND D");
    cpu->InstructionAnd(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0xa3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND E");
    cpu->InstructionAnd(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0xa4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND H");
    cpu->InstructionAnd(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0xa5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND L");
    cpu->InstructionAnd(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0xa6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "AND (HL)");
    cpu->InstructionAnd(value);
}

void CPUOperator::Op_0xa7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "AND A");
    cpu->InstructionAnd(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0xa8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR B");
    cpu->InstructionXor(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0xa9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR C");
    cpu->InstructionXor(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0xaa(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR D");
    cpu->InstructionXor(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0xab(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR E");
    cpu->InstructionXor(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0xac(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR H");
    cpu->InstructionXor(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0xad(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR L");
    cpu->InstructionXor(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0xae(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "XOR (HL)");
    cpu->InstructionXor(value);
}

void CPUOperator::Op_0xaf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "XOR A");
    cpu->InstructionXor(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0xb0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR B");
    cpu->InstructionOr(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0xb1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR C");
    cpu->InstructionOr(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0xb2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR D");
    cpu->InstructionOr(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0xb3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR E");
    cpu->InstructionOr(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0xb4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR H");
    cpu->InstructionOr(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0xb5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR L");
    cpu->InstructionOr(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0xb6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "OR (HL)");
    cpu->InstructionOr(value);
}

void CPUOperator::Op_0xb7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "OR A");
    cpu->InstructionOr(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0xb8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP B");
    cpu->InstructionCp(cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::Op_0xb9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP C");
    cpu->InstructionCp(cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::Op_0xba(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP D");
    cpu->InstructionCp(cpu->m_CPU->registers.de.hi);
}

void CPUOperator::Op_0xbb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP E");
    cpu->InstructionCp(cpu->m_CPU->registers.de.lo);
}

void CPUOperator::Op_0xbc(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP H");
    cpu->InstructionCp(cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::Op_0xbd(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP L");
    cpu->InstructionCp(cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::Op_0xbe(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "CP (HL)");
    cpu->InstructionCp(value);
}

void CPUOperator::Op_0xbf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "CP A");
    cpu->InstructionCp(cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0xc0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET NZ");
    cpu->InstructionRetCondition(mmu, CPU::Condition::NZ);
}

void CPUOperator::Op_0xc1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP BC");
    cpu->m_CPU->registers.bc.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_0xc2(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP NZ, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::NZ, 0x02);
}

void CPUOperator::Op_0xc3(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::Always, 0x02);
}

void CPUOperator::Op_0xc5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH BC");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.bc.word);
}

void CPUOperator::Op_0xc6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "ADD A, $%02x", value);
    cpu->InstructionAdd8(&cpu->m_CPU->registers.af.hi, value);
}

void CPUOperator::Op_0xc7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 00h");
    cpu->InstructionCall(mmu, 0x0000, 1);
}

void CPUOperator::Op_0xc8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET Z");
    cpu->InstructionRetCondition(mmu, CPU::Condition::Z);
}

void CPUOperator::Op_0xc9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET");
    cpu->InstructionRet(mmu);
}

void CPUOperator::Op_0xca(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP Z, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::Z, 0x02);
}

//-----------------------------------------------------------------------------

void CPUOperator::Op_0xcb(CPU* cpu, MMU* mmu)
{
    // Fetch instruction
    uint8_t op = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    cpu->m_CPU->currentOp += op;

    // Run instruction
    OpFunc_t* opfunc_cb = &m_OpCbTable[op];
    cpu->m_CPU->registers.pc.word++;

    if (!opfunc_cb || !opfunc_cb->function)
    {
        cpu->DebugOpcodeError();
        return;
    }

    (this->*opfunc_cb->function)(cpu, mmu);
    cpu->m_CPU->currentClock.m = opfunc_cb->m;
    cpu->m_CPU->currentClock.t = opfunc_cb->t;
    cpu->m_CPU->registers.pc.word += opfunc_cb->b;
}

//-----------------------------------------------------------------------------

void CPUOperator::Op_0xcd(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "CALL $%04x", addr);

    cpu->InstructionCall(mmu, addr, 2);
}

void CPUOperator::Op_0xcf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 08h");
    cpu->InstructionCall(mmu, 0x0008, 0);
}

void CPUOperator::Op_0xd0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET NC");
    cpu->InstructionRetCondition(mmu, CPU::Condition::NC);
}

void CPUOperator::Op_0xd1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP DE");
    cpu->m_CPU->registers.de.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_0xd2(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP NC, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::NC, 0x02);
}

void CPUOperator::Op_0xd5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH DE");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.de.word);
}

void CPUOperator::Op_0xd7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 10h");
    cpu->InstructionCall(mmu, 0x0010, 0);
}

void CPUOperator::Op_0xd8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RET C");
    cpu->InstructionRetCondition(mmu, CPU::Condition::C);
}

void CPUOperator::Op_0xd9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RETI");
    cpu->InstructionRet(mmu);
    cpu->m_CPU->ime = true;
}

void CPUOperator::Op_0xda(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "JP C, $%04x", addr);
    cpu->InstructionJp(addr, CPU::Condition::C, 0x02);
}

void CPUOperator::Op_0xdf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 18h");
    cpu->InstructionCall(mmu, 0x0018, 0);
}

void CPUOperator::Op_0xe0(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($FF00+$%02x), A", byte);
    uint16_t addr = 0xFF00 + byte;
    mmu->WriteByte(addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0xe1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP HL");
    cpu->m_CPU->registers.hl.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_0xe2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD ($FF00+C), A");
    uint16_t addr = 0xFF00 + cpu->m_CPU->registers.bc.lo;
    mmu->WriteByte(addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0xe5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH HL");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.hl.word);
}

void CPUOperator::Op_0xe6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "AND $%02X", value);
    cpu->InstructionAnd(value);
}

void CPUOperator::Op_0xe7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 20h");
    cpu->InstructionCall(mmu, 0x0020, 0);
}

void CPUOperator::Op_0xe8(CPU* cpu, MMU* mmu)
{
    int8_t value = (int8_t)mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "ADD SP, $%02x", value);

    cpu->FlagSetSub(false);
    cpu->FlagSetZero(false);

    cpu->FlagSetCarry(((cpu->m_CPU->registers.sp.word) + value) > 0xFFFF);
    cpu->FlagSetHalfcarry(((cpu->m_CPU->registers.sp.word & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);

    cpu->m_CPU->registers.sp.word += value;
}

void CPUOperator::Op_0xe9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "JP HL");
    cpu->InstructionJp(cpu->m_CPU->registers.hl.word, CPU::Condition::Always, 0);
}

void CPUOperator::Op_0xea(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD ($%04x), A", addr);
    mmu->WriteByte(addr, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::Op_0xef(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 28h");
    cpu->InstructionCall(mmu, 0x0028, 0);
}

void CPUOperator::Op_0xf0(CPU* cpu, MMU* mmu)
{
    uint8_t byte = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, ($FF00+$%02x)", byte);
    uint16_t addr = 0xFF00 + byte;
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(addr);
}

void CPUOperator::Op_0xf1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "POP AF");
    cpu->m_CPU->registers.af.word = mmu->ReadWord(cpu->m_CPU->registers.sp.word);
    cpu->m_CPU->registers.sp.word += 2;
}

void CPUOperator::Op_0xf2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "LD A, ($FF00+C)");
    uint16_t addr = 0xFF00 + cpu->m_CPU->registers.bc.lo;
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(addr);
}

void CPUOperator::Op_0xf3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "DI");
    cpu->m_CPU->ime = false;
}

void CPUOperator::Op_0xf5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "PUSH AF");
    cpu->m_CPU->registers.sp.word -= 2;
    mmu->WriteWord(cpu->m_CPU->registers.sp.word, cpu->m_CPU->registers.af.word);
}

void CPUOperator::Op_0xf7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 30h");
    cpu->InstructionCall(mmu, 0x0030, 0);
}

void CPUOperator::Op_0xfa(CPU* cpu, MMU* mmu)
{
    uint16_t addr = mmu->ReadWord(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "LD A, ($%04x)", addr);
    cpu->m_CPU->registers.af.hi = mmu->ReadByte(addr);
}

void CPUOperator::Op_0xfb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "EI");
    cpu->m_CPU->ime = true;
}

void CPUOperator::Op_0xfe(CPU* cpu, MMU* mmu)
{
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    DebugInstruction(cpu, mmu, "CP $%02x", v);
    cpu->InstructionCp(v);
}

void CPUOperator::Op_0xff(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RST 38h");
    cpu->InstructionCall(mmu, 0x0038, 0);
}

//-----------------------------------------------------------------------------

void CPUOperator::OpCb_0x00(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC B");
    cpu->InstructionRlc(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x01(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC C");
    cpu->InstructionRlc(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x02(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC D");
    cpu->InstructionRlc(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x03(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC E");
    cpu->InstructionRlc(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x04(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC H");
    cpu->InstructionRlc(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x05(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC L");
    cpu->InstructionRlc(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x06(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRlc(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x07(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RLC A");
    cpu->InstructionRlc(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x08(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC B");
    cpu->InstructionRrc(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x09(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC C");
    cpu->InstructionRrc(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x0a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC D");
    cpu->InstructionRrc(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x0b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC E");
    cpu->InstructionRrc(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x0c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC H");
    cpu->InstructionRrc(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x0d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC L");
    cpu->InstructionRrc(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x0e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRrc(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x0f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RRC A");
    cpu->InstructionRrc(&cpu->m_CPU->registers.af.hi);
}

// CB OP Codes

void CPUOperator::OpCb_0x10(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL B");
    cpu->InstructionRl(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x11(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL C");
    cpu->InstructionRl(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x12(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL D");
    cpu->InstructionRl(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x13(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL E");
    cpu->InstructionRl(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x14(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL H");
    cpu->InstructionRl(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x15(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL L");
    cpu->InstructionRl(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x16(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRl(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x17(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RL A");
    cpu->InstructionRl(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x18(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR B");
    cpu->InstructionRr(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x19(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR C");
    cpu->InstructionRr(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x1a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR D");
    cpu->InstructionRr(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x1b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR E");
    cpu->InstructionRr(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x1c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR H");
    cpu->InstructionRr(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x1d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR L");
    cpu->InstructionRr(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x1e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionRr(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x1f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RR A");
    cpu->InstructionRr(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x20(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA B");
    cpu->InstructionSla(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x21(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA C");
    cpu->InstructionSla(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x22(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA D");
    cpu->InstructionSla(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x23(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA E");
    cpu->InstructionSla(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x24(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA H");
    cpu->InstructionSla(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x25(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA L");
    cpu->InstructionSla(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x26(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSla(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x27(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SLA A");
    cpu->InstructionSla(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x28(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA B");
    cpu->InstructionSra(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x29(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA C");
    cpu->InstructionSra(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x2a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA D");
    cpu->InstructionSra(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x2b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA E");
    cpu->InstructionSra(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x2c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA H");
    cpu->InstructionSra(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x2d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA L");
    cpu->InstructionSra(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x2e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSra(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x2f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRA A");
    cpu->InstructionSra(&cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0x30(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP B");
    cpu->InstructionSwap(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x31(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP C");
    cpu->InstructionSwap(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x32(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP D");
    cpu->InstructionSwap(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x33(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP E");
    cpu->InstructionSwap(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x34(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP H");
    cpu->InstructionSwap(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x35(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP L");
    cpu->InstructionSwap(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x36(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSwap(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x37(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SWAP A");
    cpu->InstructionSwap(&cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x38(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL B");
    cpu->InstructionSrl(&cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x39(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL C");
    cpu->InstructionSrl(&cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x3a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL D");
    cpu->InstructionSrl(&cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x3b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL E");
    cpu->InstructionSrl(&cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x3c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL H");
    cpu->InstructionSrl(&cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x3d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL L");
    cpu->InstructionSrl(&cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x3e(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL (HL)");
    uint8_t v = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    cpu->InstructionSrl(&v);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, v);
}

void CPUOperator::OpCb_0x3f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "SRL A");
    cpu->InstructionSrl(&cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0x40(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, B");
    cpu->InstructionBit(0, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x41(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, C");
    cpu->InstructionBit(0, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x42(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, D");
    cpu->InstructionBit(0, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x43(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, E");
    cpu->InstructionBit(0, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x44(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, H");
    cpu->InstructionBit(0, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x45(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, L");
    cpu->InstructionBit(0, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x46(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 0, (HL)");
    cpu->InstructionBit(0, value);
}

void CPUOperator::OpCb_0x47(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 0, A");
    cpu->InstructionBit(0, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x48(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, B");
    cpu->InstructionBit(1, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x49(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, C");
    cpu->InstructionBit(1, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x4a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, D");
    cpu->InstructionBit(1, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x4b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, E");
    cpu->InstructionBit(1, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x4c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, H");
    cpu->InstructionBit(1, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x4d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, L");
    cpu->InstructionBit(1, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x4e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 1, (HL)");
    cpu->InstructionBit(1, value);
}

void CPUOperator::OpCb_0x4f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 1, A");
    cpu->InstructionBit(1, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0x50(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, B");
    cpu->InstructionBit(2, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x51(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, C");
    cpu->InstructionBit(2, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x52(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, D");
    cpu->InstructionBit(2, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x53(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, E");
    cpu->InstructionBit(2, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x54(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, H");
    cpu->InstructionBit(2, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x55(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, L");
    cpu->InstructionBit(2, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x56(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 2, (HL)");
    cpu->InstructionBit(2, value);
}

void CPUOperator::OpCb_0x57(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 2, A");
    cpu->InstructionBit(2, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x58(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, B");
    cpu->InstructionBit(3, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x59(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, C");
    cpu->InstructionBit(3, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x5a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, D");
    cpu->InstructionBit(3, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x5b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, E");
    cpu->InstructionBit(3, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x5c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, H");
    cpu->InstructionBit(3, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x5d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, L");
    cpu->InstructionBit(3, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x5e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 3, (HL)");
    cpu->InstructionBit(3, value);
}

void CPUOperator::OpCb_0x5f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 3, A");
    cpu->InstructionBit(3, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0x60(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, B");
    cpu->InstructionBit(4, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x61(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, C");
    cpu->InstructionBit(4, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x62(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, D");
    cpu->InstructionBit(4, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x63(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, E");
    cpu->InstructionBit(4, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x64(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, H");
    cpu->InstructionBit(4, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x65(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, L");
    cpu->InstructionBit(4, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x66(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 4, (HL)");
    cpu->InstructionBit(4, value);
}

void CPUOperator::OpCb_0x67(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 4, A");
    cpu->InstructionBit(4, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x68(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, B");
    cpu->InstructionBit(5, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x69(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, C");
    cpu->InstructionBit(5, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x6a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, D");
    cpu->InstructionBit(5, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x6b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, E");
    cpu->InstructionBit(5, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x6c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, H");
    cpu->InstructionBit(5, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x6d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, L");
    cpu->InstructionBit(5, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x6e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 5, (HL)");
    cpu->InstructionBit(5, value);
}

void CPUOperator::OpCb_0x6f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 5, A");
    cpu->InstructionBit(5, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0x70(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, B");
    cpu->InstructionBit(6, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x71(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, C");
    cpu->InstructionBit(6, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x72(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, D");
    cpu->InstructionBit(6, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x73(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, E");
    cpu->InstructionBit(6, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x74(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, H");
    cpu->InstructionBit(6, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x75(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, L");
    cpu->InstructionBit(6, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x76(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 6, (HL)");
    cpu->InstructionBit(6, value);
}

void CPUOperator::OpCb_0x77(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 6, A");
    cpu->InstructionBit(6, cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x78(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, B");
    cpu->InstructionBit(7, cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x79(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, C");
    cpu->InstructionBit(7, cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x7a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, D");
    cpu->InstructionBit(7, cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x7b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, E");
    cpu->InstructionBit(7, cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x7c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, H");
    cpu->InstructionBit(7, cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x7d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, L");
    cpu->InstructionBit(7, cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x7e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "BIT 7, (HL)");
    cpu->InstructionBit(7, value);
}

void CPUOperator::OpCb_0x7f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "BIT 7, A");
    cpu->InstructionBit(7, cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0x80(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, B");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x81(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, C");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x82(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, D");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x83(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, E");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x84(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, H");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x85(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, L");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x86(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 0, (HL)");
    cpu->InstructionRes(0, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0x87(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 0, A");
    cpu->InstructionRes(0, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x88(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, B");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x89(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, C");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x8a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, D");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x8b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, E");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x8c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, H");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x8d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, L");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x8e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 1, (HL)");
    cpu->InstructionRes(1, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0x8f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 1, A");
    cpu->InstructionRes(1, &cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0x90(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, B");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x91(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, C");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x92(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, D");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x93(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, E");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x94(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, H");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x95(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, L");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x96(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 2, (HL)");
    cpu->InstructionRes(2, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0x97(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 2, A");
    cpu->InstructionRes(2, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0x98(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, B");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0x99(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, C");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0x9a(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, D");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0x9b(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, E");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0x9c(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, H");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0x9d(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, L");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0x9e(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 3, (HL)");
    cpu->InstructionRes(3, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0x9f(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 3, A");
    cpu->InstructionRes(3, &cpu->m_CPU->registers.af.hi);
}


void CPUOperator::OpCb_0xa0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, B");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0xa1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, C");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0xa2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, D");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0xa3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, E");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0xa4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, H");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0xa5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, L");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0xa6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 4, (HL)");
    cpu->InstructionRes(4, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0xa7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 4, A");
    cpu->InstructionRes(4, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0xa8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, B");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0xa9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, C");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0xaa(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, D");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0xab(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, E");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0xac(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, H");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0xad(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, L");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0xae(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 5, (HL)");
    cpu->InstructionRes(5, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0xaf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 5, A");
    cpu->InstructionRes(5, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0xb0(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, B");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0xb1(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, C");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0xb2(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, D");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0xb3(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, E");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0xb4(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, H");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0xb5(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, L");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0xb6(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 6, (HL)");
    cpu->InstructionRes(6, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0xb7(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 6, A");
    cpu->InstructionRes(6, &cpu->m_CPU->registers.af.hi);
}

void CPUOperator::OpCb_0xb8(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, B");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.bc.hi);
}

void CPUOperator::OpCb_0xb9(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, C");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.bc.lo);
}

void CPUOperator::OpCb_0xba(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, D");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.de.hi);
}

void CPUOperator::OpCb_0xbb(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, E");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.de.lo);
}

void CPUOperator::OpCb_0xbc(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, H");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.hl.hi);
}

void CPUOperator::OpCb_0xbd(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, L");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.hl.lo);
}

void CPUOperator::OpCb_0xbe(CPU* cpu, MMU* mmu)
{
    uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.hl.word);
    DebugInstruction(cpu, mmu, "RES 7, (HL)");
    cpu->InstructionRes(7, &value);
    mmu->WriteByte(cpu->m_CPU->registers.hl.word, value);
}

void CPUOperator::OpCb_0xbf(CPU* cpu, MMU* mmu)
{
    DebugInstruction(cpu, mmu, "RES 7, A");
    cpu->InstructionRes(7, &cpu->m_CPU->registers.af.hi);
}

//-----------------------------------------------------------------------------

void CPUOperator::Op_0x10(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xc4(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xcc(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xce(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xd4(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xd6(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xdc(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xde(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xee(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xf6(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::Op_0xf8(CPU* cpu, MMU* mmu)
{
    const uint8_t value = mmu->ReadByte(cpu->m_CPU->registers.pc.word);
    const uint16_t reg = cpu->m_CPU->registers.sp.word;

    DebugInstruction(cpu, mmu, "LD (HL)");

    uint16_t result = static_cast<uint16_t>(reg + value);

    cpu->FlagSetZero(false);
    cpu->FlagSetSub(false);
    cpu->FlagSetHalfcarry(((reg ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10);
    cpu->FlagSetCarry(((reg ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100);

    cpu->m_CPU->registers.hl.word = result;
}
void CPUOperator::Op_0xf9(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc0(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc1(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc2(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc3(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc4(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc5(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc6(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc7(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc8(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xc9(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xca(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xcb(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xcc(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xcd(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xce(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xcf(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd0(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd1(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd2(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd3(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd4(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd5(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd6(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd7(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd8(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xd9(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xda(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xdb(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xdc(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xdd(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xde(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xdf(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe0(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe1(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe2(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe3(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe4(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe5(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe6(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe7(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe8(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xe9(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xea(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xeb(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xec(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xed(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xee(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xef(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf0(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf1(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf2(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf3(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf4(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf5(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf6(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf7(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf8(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xf9(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xfa(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xfb(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xfc(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xfd(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xfe(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }
void CPUOperator::OpCb_0xff(CPU* cpu, MMU* mmu) { DebugOpcodeError(cpu); }

//-----------------------------------------------------------------------------

void CPUOperator::DebugOpcodeError(CPU* cpu)
{

}

void CPUOperator::DebugInstruction(CPU* cpu, MMU* mmu, const char* disassembly, ...)
{

}

void CPUOperator::DebugInterruption(CPU* cpu, MMU* mmu, const char* message, ...)
{

}

} // namespace gbl
