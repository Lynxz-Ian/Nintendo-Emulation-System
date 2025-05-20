#pragma once
#include <cstdint>
#include <memory>
#include "Cartridge.h"


class NES2C02
{
    public:
        NES2C02();
        ~NES2C02();

        private:
        uint8_t     tb1Name[2][1024] = {};
        uint8_t     tb1Palette[32];
        uint8_t     tb1Pattern[2][4096]; // Future use, unused on NES
        uint8_t     tblName[2][1024] = {}; // 2KB for nametables

    public:

        // Communicate with Main Bus
        uint8_t cpuRead(uint16_t addr, bool rdonly = false);
        void    cpuWrite(uint16_t addr, uint8_t data);

        // Communicate with PPU Bus
        uint8_t ppuRead(uint16_t addr, bool rdonly = false);
        void    ppuWrite(uint16_t addr, uint8_t data);

private:
    // The Cartridge/GamePak
    std::shared_ptr<Cartridge> cart;

public:
    // Interface
    void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
};