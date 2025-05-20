#include "../Header Files/Mapper_004.h"

Mapper_004::Mapper_004(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks) {}

Mapper_004::~Mapper_004() {}

bool Mapper_004::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        if (prgMode)
            mapped_addr = (nPRGBanks - 2) * 0x2000 + (addr & 0x1FFF);
        else
            mapped_addr = prgBankSelect[0] * 0x2000 + (addr & 0x1FFF);
        return true;
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        mapped_addr = prgBankSelect[1] * 0x2000 + (addr & 0x1FFF);
        return true;
    }
    else if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        if (prgMode)
            mapped_addr = prgBankSelect[0] * 0x2000 + (addr & 0x1FFF);
        else
            mapped_addr = (nPRGBanks - 2) * 0x2000 + (addr & 0x1FFF);
        return true;
    }
    else if (addr >= 0xE000 && addr <= 0xFFFF)
    {
        mapped_addr = (nPRGBanks - 1) * 0x2000 + (addr & 0x1FFF);
        return true;
    }
    return false;
}

bool Mapper_004::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data)
{
    if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        mirror = (data & 0x01) ? Mirror::HORIZONTAL : Mirror::VERTICAL;
    }
    return false;
}

bool Mapper_004::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_004::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr < 0x2000)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

Mirror Mapper_004::getMirror() const
{
    return mirror;
}