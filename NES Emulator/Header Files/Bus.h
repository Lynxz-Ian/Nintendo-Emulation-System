#pragma once
#include <cstdint>
#include "CPU6502.h"
#include <array>

class Bus
{
public:
        Bus();
        ~Bus();

public: // Devices on bus
    CPU6502 cpu;

// Fake RAM for testing
    uint8_t ram[64 * 1024] = {0}; // 64KB RAM, zero-initialized

 // Bus Read & Write
    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr, bool bReadOnly = false);
    
};