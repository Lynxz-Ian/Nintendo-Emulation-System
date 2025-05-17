#pragma once
#include <cstdint>
#include "6502.h"
#include <array>

class Bus
{
public:
        Bus();
        ~Bus();

public: // Devices on bus
    6502 cpu;

// Fake RAM for testing
    std::array<uint8_t, 64* 1024> ram;

public: // Bus Read & Write
    void Write(uint16_t addr, uint8_t data);
    uint8_t Read(uint16_t addr, bool bReadOnly = false);
    
};