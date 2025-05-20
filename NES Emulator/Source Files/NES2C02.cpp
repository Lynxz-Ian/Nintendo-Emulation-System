#include "../Header Files/NES2C02.h"
#include "../Header Files/Mapper_001.h" // For Mirror enum, or include where Mirror is defined

// Add a 2KB VRAM array for nametables in NES2C02.h:
// uint8_t tblName[2][1024]; // Or: uint8_t vram[2048];

NES2C02::NES2C02()
{
    // Constructor: Initialize PPU state here if needed
}

NES2C02::~NES2C02()
{
    // Destructor: Clean up if needed
}


uint8_t NES2C02::cpuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;

    switch (addr)
    {
        case 0x2000: // Control Register
            //data = control;
            break;
        case 0x2001: // Mask Register
            //data = mask;
            break;
        case 0x2002: // Status Register
            //data = status;
            break;
        case 0x2003: // OAM Address
            //data = oam_addr;
            break;
        case 0x2004: // OAM Data
            //data = oam_data;
            break;
        case 0x2005: // Scroll
            //data = scroll;
            break;
        case 0x2006: // Address
            //data = address;
            break;
        case 0x2007: // Data
            //data = data_buffer;
            break;
        default:
            // Handle other addresses if needed
            break;
    }

    return data;
}

void NES2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
        case 0x0000: // Control Register
            //control = data;
            break;
        case 0x0001: // Mask Register
           // mask = data;
            break;
        case 0x0002:                   //Status Register
                                    // Status is read-only, ignore write
            break;
        case 0x0003:                // OAM Address
            //oam_addr = data;
            break;
        case 0x0004:                // OAM Data
            //oam_data = data;
            break;
        case 0x0005: // Scroll
            //scroll = data;
            break;
        case 0x0006: // Address
            //address = data;
            break;
        case 0x0007: // Data
            //data_buffer = data;
            break;
        default:
            // Handle other addresses if needed
            break;
    }
}

uint8_t NES2C02::ppuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;
    addr &= 0x3FFF; // Mask to 14 bits

    // Pattern tables and cartridge CHR
    if (addr < 0x2000)
    {
        if (cart && cart->ppuRead(addr, data))
        {
            // Data loaded from cartridge CHR ROM/RAM
        }
    }
    // Nametables
    else if (addr >= 0x2000 && addr <= 0x3EFF)
    {
        uint16_t vram_addr = addr & 0x0FFF; // $2000-$2FFF mirrored every 4KB

        // Default to horizontal if no mapper
        Mirror mirroring = Mirror::HORIZONTAL;
        if (cart && cart->mapper)
            mirroring = cart->mapper->getMirror();

        switch (mirroring)
        {
            case Mirror::VERTICAL:
                // $2000/$2800 -> tblName[0], $2400/$2C00 -> tblName[1]
                data = tblName[(vram_addr & 0x0800) >> 11][vram_addr & 0x03FF];
                break;
            case Mirror::HORIZONTAL:
                // $2000/$2400 -> tblName[0], $2800/$2C00 -> tblName[1]
                data = tblName[(vram_addr & 0x0400) >> 10][vram_addr & 0x03FF];
                break;
            case Mirror::SINGLE_SCREEN_LOW:
                data = tblName[0][vram_addr & 0x03FF];
                break;
            case Mirror::SINGLE_SCREEN_HIGH:
                data = tblName[1][vram_addr & 0x03FF];
                break;
            default:
                data = 0x00; // Fallback for unknown mirroring
                break;
        }
    }
    // Palette RAM, etc. (not shown here)
    // else if (addr >= 0x3F00 && addr <= 0x3FFF) { ... }

    return data;
}

void NES2C02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF; // Mask to 14 bits

    // Pattern tables and cartridge CHR
    if (addr < 0x2000)
    {
        if (cart)
            cart->ppuWrite(addr, data);
    }
    // Nametables
    else if (addr >= 0x2000 && addr <= 0x3EFF)
    {
        uint16_t vram_addr = addr & 0x0FFF; // $2000-$2FFF mirrored every 4KB

        Mirror mirroring = Mirror::HORIZONTAL;
        if (cart && cart->mapper)
            mirroring = cart->mapper->getMirror();

        switch (mirroring)
        {
            case Mirror::VERTICAL:
                tblName[(vram_addr & 0x0800) >> 11][vram_addr & 0x03FF] = data;
                break;
            case Mirror::HORIZONTAL:
                tblName[(vram_addr & 0x0400) >> 10][vram_addr & 0x03FF] = data;
                break;
            case Mirror::SINGLE_SCREEN_LOW:
                tblName[0][vram_addr & 0x03FF] = data;
                break;
            case Mirror::SINGLE_SCREEN_HIGH:
                tblName[1][vram_addr & 0x03FF] = data;
                break;
            default:
                // Ignore write for unknown mirroring
                break;
        }
    }
    // Palette RAM, etc. (not shown here)
    // else if (addr >= 0x3F00 && addr <= 0x3FFF) { ... }
}

void NES2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
}