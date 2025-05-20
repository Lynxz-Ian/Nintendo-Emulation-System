#pragma once
#include "../Header Files/Mapper.h"


class Mapper_066 : public Mapper
{
public:
    Mapper_066(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper_066();

    bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data) override;
    bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;

    Mirror getMirror() const override;

private:
    uint8_t prgBank = 0;
    uint8_t chrBank = 0;
    Mirror mirror = Mirror::VERTICAL; // Mapper 66 is always vertical mirroring
};