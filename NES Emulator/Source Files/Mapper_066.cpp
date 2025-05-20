#include "../Header Files/Mapper_066.h"

Mapper_066::Mapper_066(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
    prgBank = 0;
    chrBank = 0;
    mirror = Mirror::VERTICAL;
}

Mapper_066::~Mapper_066() {}

bool Mapper_066::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // GxROM: PRG ROM $8000-$FFFF, 32KB bank switching
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = (prgBank * 0x8000) + (addr & 0x7FFF);
        return true;
    }
    return false;
}

bool Mapper_066::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    // GxROM: Writing to $8000-$FFFF sets PRG and CHR bank
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        prgBank = (data & 0x30) >> 4;
        chrBank = (data & 0x03);
    }
    return false;
}

bool Mapper_066::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // GxROM: CHR ROM $0000-$1FFF, 8KB bank switching
    if (addr < 0x2000)
    {
        mapped_addr = (chrBank * 0x2000) + (addr & 0x1FFF);
        return true;
    }
    return false;
}

bool Mapper_066::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    // Usually CHR ROM, but if CHR RAM is present, allow writes
    if (addr < 0x2000)
    {
        mapped_addr = (chrBank * 0x2000) + (addr & 0x1FFF);
        return true;
    }
    return false;
}

Mirror Mapper_066::getMirror() const
{
    return mirror;
}