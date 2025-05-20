#pragma once
#include <cstdint>
#include "Mapper.h"

class Mapper_004 : public Mapper
{
public:
    Mapper_004(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper_004();

    bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data) override;
    bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;

    Mirror getMirror() const override;

private:
    uint8_t prgBankSelect[2] = {0, 0};
    uint8_t chrBankSelect[6] = {0, 0, 0, 0, 0, 0};
    uint8_t bankSelect = 0;
    uint8_t prgMode = 0;
    uint8_t chrMode = 0;
    Mirror mirror = Mirror::HORIZONTAL;
};