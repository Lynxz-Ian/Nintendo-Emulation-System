#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include "CPU6502.h"
#include "NES2C02.h"

#include "Cartridge.h"

class Bus
{
public:
        Bus();
        ~Bus();
private:
    std::shared_ptr<Cartridge> cart;
    
public: // Devices on bus
    CPU6502 cpu;

    // The 2C02 Picture Processing Unit (PPU)
    NES2C02 ppu;

    // Fake RAM for testing
    std::array<uint8_t, 2048> cpuRam; // 2KB of RAM

    // The Cartridge/GamePak
    std::shared_ptr<Cartridge> cartridge;


    // Bus Read & Write
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public: // System Interface
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void reset();
    void clock();

private:
    // Counter for how many cycles have elapsed
    uint32_t nSystemClockCounter = 0;
};