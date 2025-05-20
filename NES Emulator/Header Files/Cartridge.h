#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "Mapper.h"


class Cartridge
{
public:
    Cartridge(const std::string& sFileName);
    ~Cartridge();

    bool cpuRead(uint16_t addr, uint8_t& data);

    bool cpuWrite(uint16_t addr, uint8_t data);
    bool ppuRead(uint16_t addr, uint8_t& data);
    bool ppuWrite(uint16_t addr, uint8_t data);

    // Mapper pointer (public for access by Bus/PPU)
    std::shared_ptr<Mapper> mapper;

    // Accessors for PRG/CHR memory (optional, for debugging or direct access)
    const std::vector<uint8_t>& PRGMemory() const { return vPRGMemory; }
    const std::vector<uint8_t>& CHRMemory() const { return vCHRMemory; }
    uint8_t MapperID() const { return nMapperID; }
    uint8_t PRGBanks() const { return nPRGBanks; }
    uint8_t CHRBanks() const { return nCHRBanks; }

private:
    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;

    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};

