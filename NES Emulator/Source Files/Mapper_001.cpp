#include "../Header Files/Mapper_001.h"

Mapper_001::Mapper_001(uint8_t prgBanks, uint8_t chrBanks)
    : Mapper(prgBanks, chrBanks) {}
Mapper_001::~Mapper_001() {}

bool Mapper_001::cpuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        if ((control & 0x08) == 0x08)
        {
            // 16KB PRG mode
            if (addr < 0xC000)
                mapped_addr = (prgBank * 0x4000) + (addr & 0x3FFF);
            else
                mapped_addr = ((nPRGBanks - 1) * 0x4000) + (addr & 0x3FFF);
        }
        else
        {
            // 32KB PRG mode
            mapped_addr = ((prgBank >> 1) * 0x8000) + (addr & 0x7FFF);
        }
        return true;
    }
    return false;
}

bool Mapper_001::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        if (data & 0x80)
        {
            shiftRegister = 0x10;
            control |= 0x0C;
        }
        else
        {
            bool complete = shiftRegister & 1;
            shiftRegister = (shiftRegister >> 1) | ((data & 1) << 4);

            if (complete)
            {
                uint16_t reg = (addr >> 13) & 0x03;
                switch (reg)
                {
                    case 0: // Control
                        control = shiftRegister & 0x1F;
                        switch (control & 0x03)
                        {
                            case 0: mirror = Mirror::SINGLE_SCREEN_LOW; break;
                            case 1: mirror = Mirror::SINGLE_SCREEN_HIGH; break;
                            case 2: mirror = Mirror::VERTICAL; break;
                            case 3: mirror = Mirror::HORIZONTAL; break;
                        }
                        break;
                    case 1: // CHR bank 0
                        chrBank0 = shiftRegister & 0x1F;
                        break;
                    case 2: // CHR bank 1
                        chrBank1 = shiftRegister & 0x1F;
                        break;
                    case 3: // PRG bank
                        prgBank = shiftRegister & 0x0F;
                        break;
                }
                shiftRegister = 0x10;
            }
        }
        return false;
    }
    return false;
}

bool Mapper_001::ppuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr < 0x2000)
    {
        if ((control & 0x10) == 0x10)
        {
            // 4KB CHR banking
            if (addr < 0x1000)
                mapped_addr = chrBank0 * 0x1000 + (addr & 0x0FFF);
            else
                mapped_addr = chrBank1 * 0x1000 + (addr & 0x0FFF);
        }
        else
        {
            // 8KB CHR banking
            mapped_addr = (chrBank0 >> 1) * 0x2000 + (addr & 0x1FFF);
        }
        return true;
    }
    return false;
}

bool Mapper_001::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr < 0x2000)
    {
        if ((control & 0x10) == 0x10)
        {
            if (addr < 0x1000)
                mapped_addr = chrBank0 * 0x1000 + (addr & 0x0FFF);
            else
                mapped_addr = chrBank1 * 0x1000 + (addr & 0x0FFF);
        }
        else
        {
            mapped_addr = (chrBank0 >> 1) * 0x2000 + (addr & 0x1FFF);
        }
        return true;
    }
    return false;
}

Mirror Mapper_001::getMirror() const { return mirror; }

// Add these private members to your Mapper_001 class in the header:
// uint8_t shiftRegister = 0x10;
// uint8_t control = 0x1C;
// uint8_t prgBank = 0;
// uint8_t chrBank0 = 0;
// uint8_t chrBank1 = 0;