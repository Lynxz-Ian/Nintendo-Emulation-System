#include "../Header Files/Mapper_002.h"

Mapper_002::Mapper_002(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {}

Mapper_002::~Mapper_002() {}

bool Mapper_002::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // UxROM: PRG ROM $8000-$FFFF
    if (addr >= 0x8000 && addr <= 0xBFFF)
    {
        // Switchable 16KB bank
        mapped_addr = prgBankSelect * 0x4000 + (addr & 0x3FFF);
        return true;
    }
    else if (addr >= 0xC000 && addr <= 0xFFFF)
    {
        // Fixed to last 16KB bank
        mapped_addr = (nPRGBanks - 1) * 0x4000 + (addr & 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_002::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    // UxROM: Writing to $8000-$FFFF sets PRG bank
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        prgBankSelect = data & 0x0F;
    }
    return false;
}

bool Mapper_002::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // CHR ROM/RAM $0000-$1FFF
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_002::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    // CHR RAM support (if present)
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}