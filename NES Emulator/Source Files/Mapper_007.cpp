#include "../Header Files/Mapper_007.h"

Mapper_007::Mapper_007(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {}

Mapper_007::~Mapper_007() {}

bool Mapper_007::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // AOROM: PRG ROM $8000-$FFFF, 32KB bank switching
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = (prgBank * 0x8000) + (addr & 0x7FFF);
        return true;
    }
    return false;
}

bool Mapper_007::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    // AOROM: Writing to $8000-$FFFF sets PRG bank and mirroring
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        prgBank = data & 0x07;
        mirror = (data & 0x10) ? Mirror::SINGLE_SCREEN_HIGH : Mirror::SINGLE_SCREEN_LOW;
    }
    return false;
}

bool Mapper_007::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // AOROM: CHR is always RAM, not ROM, so just map directly
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_007::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

Mirror Mapper_007::getMirror() const
{
    return mirror;
}