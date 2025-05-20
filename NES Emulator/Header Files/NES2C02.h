#pragma once
#include <cstdint>
#include <memory>
#include "Cartridge.h"


class NES2C02
{
public:
    NES2C02();
    ~NES2C02();

    // Communicate with Main Bus
    uint8_t cpuRead(uint16_t addr, bool rdonly = false);
    void    cpuWrite(uint16_t addr, uint8_t data);

    // Communicate with PPU Bus
    uint8_t ppuRead(uint16_t addr, bool rdonly = false);
    void    ppuWrite(uint16_t addr, uint8_t data);

    void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();

    bool frame_complete = false;

    // For NMI signaling
    bool nmi = false;

    // Simple framebuffer (256x240)
    uint32_t frame[256 * 240] = {};

private:
    std::shared_ptr<Cartridge> cart;

    // PPU memory
    uint8_t tblName[2][1024] = {};
    uint8_t tblPalette[32] = {};

    // Registers
    uint8_t status = 0x00;
    uint8_t mask   = 0x00;
    uint8_t control= 0x00;

    // Loopy registers for scrolling
    uint16_t vram_addr = 0x0000;
    uint16_t tram_addr = 0x0000;

    // Fine X scroll
    uint8_t fine_x = 0x00;

    // Internal
    uint8_t address_latch = 0x00;
    uint8_t ppu_data_buffer = 0x00;

    // Scanline/cycle
    int16_t scanline = 0;
    int16_t cycle = 0;

    // Background shifters
    uint16_t bg_shifter_pattern_lo = 0;
    uint16_t bg_shifter_pattern_hi = 0;
    uint16_t bg_shifter_attrib_lo = 0;
    uint16_t bg_shifter_attrib_hi = 0;

    // Background fetch latches
    uint8_t bg_next_tile_id = 0;
    uint8_t bg_next_tile_attrib = 0;
    uint8_t bg_next_tile_lsb = 0;
    uint8_t bg_next_tile_msb = 0;

    // Palette helper
    uint8_t GetColorFromPaletteRam(uint8_t palette, uint8_t pixel);
};