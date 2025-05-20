#include "../Header Files/Mapper_071.h"

Mapper_071::Mapper_071(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
    prgBank = 0;
    mirror = Mirror::VERTICAL;
}

Mapper_071::~Mapper_071() {}

bool Mapper_071::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // Camerica/Codemasters: PRG ROM $8000-$FFFF, 16KB bank switching
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = (prgBank * 0x4000) + (addr & 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_071::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    // Writing to $8000-$FFFF sets PRG bank (4 bits)
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        prgBank = data & 0x0F;
    }
    return false;
}

bool Mapper_071::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // CHR ROM $0000-$1FFF, no banking (fixed)
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_071::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    // Usually CHR ROM, but if CHR RAM is present, allow writes
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

Mirror Mapper_071::getMirror() const
{
    return mirror;
}