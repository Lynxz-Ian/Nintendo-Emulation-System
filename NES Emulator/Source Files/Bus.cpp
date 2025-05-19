#include "../Header Files/Bus.h"



Bus::Bus()
{
    // Clear RAM just in case
    for (auto &i : ram) i = 0x00;
    
    // Connect CPU to comm. bus
    cpu.ConnectBus(this);
}


Bus::~Bus()
{
    // Nothing to do here
}

uint8_t Bus::Read(uint16_t addr, bool readOnly) {
    if (addr <= 0x0000 && addr <= 0x1FFF) {
        // Mirror RAM every 0x0800
        return ram[addr & 0x07FF];
    }
    // TODO: Add other device reads (PPU, APU, etc.)
    return ram[addr];
}

void Bus::Write(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        // Mirror RAM every 0x0800
        ram[addr & 0x07FF] = data;
    }
    // TODO: Add other device writes (PPU, APU, etc.)
}