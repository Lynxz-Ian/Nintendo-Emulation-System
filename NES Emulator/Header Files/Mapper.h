#pragma once
#include <cstdint>

enum class Mirror
{
    HORIZONTAL,
    VERTICAL,
    SINGLE_SCREEN_LOW,
    SINGLE_SCREEN_HIGH
};

class Mapper {
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks) : nPRGBanks(prgBanks), nCHRBanks(chrBanks) {}
    virtual ~Mapper() = default;

    virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data) = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;
    virtual Mirror getMirror() const { return Mirror::HORIZONTAL; }

protected:
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};