#include "../Header Files/Mapper_003.h"

Mapper_003::Mapper_003(uint8_t prgBanks, uint8_t chrBanks)
    : Mapper(prgBanks, chrBanks) {}
    
Mapper_003::~Mapper_003() {}

bool Mapper_003::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // CNROM: PRG ROM $8000-$FFFF, fixed mapping
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_003::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    // CNROM: Writing to $8000-$FFFF selects CHR bank
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        chrBankSelect = data & 0x03;
    }
    return false;
}

bool Mapper_003::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // CHR ROM $0000-$1FFF, banked
    if (addr < 0x2000)
    {
        mapped_addr = chrBankSelect * 0x2000 + (addr & 0x1FFF);
        return true;
    }
    return false;
}

bool Mapper_003::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    // Usually CHR ROM, but if CHR RAM is present, allow writes
    if (addr < 0x2000)
    {
        mapped_addr = chrBankSelect * 0x2000 + (addr & 0x1FFF);
        return true;
    }
    return false;
}