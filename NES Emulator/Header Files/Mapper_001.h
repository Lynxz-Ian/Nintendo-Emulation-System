#pragma once
#include "../Header Files/Mapper.h"


class Mapper_001 : public Mapper
{
public:
    Mapper_001(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper_001();

    bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data) override;
    bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;

    Mirror getMirror() const override;

private:
    uint8_t shiftRegister = 0x10;
    uint8_t control = 0x1C;
    uint8_t prgBank = 0;
    uint8_t chrBank0 = 0;
    uint8_t chrBank1 = 0;
    Mirror mirror = Mirror::HORIZONTAL;
};