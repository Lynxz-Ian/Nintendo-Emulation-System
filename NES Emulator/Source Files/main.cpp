#include <sstream>
#include <string>
#include <iostream>
#include "../Header Files/Bus.h"

int main()
{
    Bus nes;

   // Load test program at $8000
   /*
    std::stringstream ss;
    ss << "A9 01 8D 00 02 A9 05 6D 00 02 8D 01 02 00";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        if (b.empty()) continue;
        nes.ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
    }

    // Set reset vector to $8000 BEFORE cpu.reset()
    nes.ram[0xFFFC] = 0x00;
    nes.ram[0xFFFD] = 0x80;

    nes.cpu.reset();
    std::cout << "PC after reset: $" << std::hex << nes.cpu.pc << std::endl;

    // Run CPU until BRK (opcode 0x00)
    bool running = true;
    while (running) {
        uint8_t opcode = nes.Read(nes.cpu.pc, true);
        if (opcode == 0x00)
            running = false;
        do {
            nes.cpu.clock();
        } while (!nes.cpu.complete());
    }

    //Print results
    std::cout << "RAM[0x0200] = $" << std::hex << (int)nes.ram[0x0200] << std::endl;
    std::cout << "RAM[0x0201] = $" << std::hex << (int)nes.ram[0x0201] << std::endl;
    std::cout << "A = $" << std::hex << (int)nes.cpu.a << std::endl;
    */
   
    return 0;
}