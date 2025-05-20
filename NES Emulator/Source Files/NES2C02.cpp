#include "../Header Files/NES2C02.h"
#include <cstring>

NES2C02::NES2C02() {}
NES2C02::~NES2C02() {}

void NES2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    cart = cartridge;
}

uint8_t NES2C02::cpuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
    switch (addr & 0x0007) {
        case 0x0002: // PPUSTATUS
            data = (status & 0xE0) | (ppu_data_buffer & 0x1F);
            status &= ~0x80; // Clear VBlank
            address_latch = 0;
            break;
        case 0x0007: // PPUDATA
            data = ppu_data_buffer;
            ppu_data_buffer = ppuRead(vram_addr);
            if (vram_addr >= 0x3F00) data = ppu_data_buffer; // No delay for palette
            vram_addr += (control & 0x04) ? 32 : 1;
            break;
        default:
            break;
    }
    return data;
}

void NES2C02::cpuWrite(uint16_t addr, uint8_t data) {
    switch (addr & 0x0007) {
        case 0x0000: // PPUCTRL
            control = data;
            tram_addr = (tram_addr & 0xF3FF) | ((data & 0x03) << 10);
            break;
        case 0x0001: // PPUMASK
            mask = data;
            break;
        case 0x0003: // OAMADDR (not implemented)
            break;
        case 0x0005: // PPUSCROLL
            if (address_latch == 0) {
                fine_x = data & 0x07;
                tram_addr = (tram_addr & 0xFFE0) | (data >> 3);
                address_latch = 1;
            } else {
                tram_addr = (tram_addr & 0x8FFF) | ((data & 0x07) << 12);
                tram_addr = (tram_addr & 0xFC1F) | ((data & 0xF8) << 2);
                address_latch = 0;
            }
            break;
        case 0x0006: // PPUADDR
            if (address_latch == 0) {
                tram_addr = (tram_addr & 0x00FF) | ((data & 0x3F) << 8);
                address_latch = 1;
            } else {
                tram_addr = (tram_addr & 0xFF00) | data;
                vram_addr = tram_addr;
                address_latch = 0;
            }
            break;
        case 0x0007: // PPUDATA
            ppuWrite(vram_addr, data);
            vram_addr += (control & 0x04) ? 32 : 1;
            break;
        default:
            break;
    }
}

uint8_t NES2C02::ppuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;
    if (cart && cart->ppuRead(addr, data)) {
        // CHR ROM/RAM
    } else if (addr >= 0x2000 && addr <= 0x3EFF) {
        data = tblName[(addr & 0x0FFF) / 0x0400][addr & 0x03FF];
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        data = tblPalette[addr & 0x1F];
    }
    return data;
}

void NES2C02::ppuWrite(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;
    if (cart && cart->ppuWrite(addr, data)) {
        // CHR ROM/RAM
    } else if (addr >= 0x2000 && addr <= 0x3EFF) {
        tblName[(addr & 0x0FFF) / 0x0400][addr & 0x03FF] = data;
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        tblPalette[addr & 0x1F] = data;
    }
}

uint8_t NES2C02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel) {
    return ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F;
}

void NES2C02::clock() {
    // Background rendering (simplified)
    if (scanline >= 0 && scanline < 240 && cycle > 0 && cycle <= 256) {
        // Fetch tile every 8 cycles
        if ((cycle - 1) % 8 == 0) {
            uint16_t v = vram_addr;
            bg_next_tile_id = ppuRead(0x2000 | (v & 0x0FFF));
            bg_next_tile_attrib = ppuRead(0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07));
            uint8_t fine_y = (v >> 12) & 0x7;
            uint16_t pattern_addr = ((control & 0x10) << 8) + (bg_next_tile_id << 4) + fine_y;
            bg_next_tile_lsb = ppuRead(pattern_addr);
            bg_next_tile_msb = ppuRead(pattern_addr + 8);
        }
        // Shift background shifters
        bg_shifter_pattern_lo <<= 1;
        bg_shifter_pattern_hi <<= 1;
        bg_shifter_attrib_lo <<= 1;
        bg_shifter_attrib_hi <<= 1;

        // Render pixel
        int px = cycle - 1;
        int py = scanline;
        uint8_t bit_mux = 0x8000 >> fine_x;
        uint8_t p0 = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1 = (bg_shifter_pattern_hi & bit_mux) > 0;
        uint8_t palette = (bg_shifter_attrib_lo & bit_mux) > 0 | ((bg_shifter_attrib_hi & bit_mux) > 0) << 1;
        uint8_t color = GetColorFromPaletteRam(palette, (p1 << 1) | p0);
        frame[py * 256 + px] = color; // You can map this to RGB later
    }

    // Advance cycle/scanline
    cycle++;
    if (cycle >= 341) {
        cycle = 0;
        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frame_complete = true;
        }
    }

    // VBlank logic
    if (scanline == 241 && cycle == 1) {
        status |= 0x80; // Set VBlank
        if (control & 0x80) nmi = true;
    }
    if (scanline == -1 && cycle == 1) {
        status &= ~0x80; // Clear VBlank
        frame_complete = false;
        nmi = false;
    }
}