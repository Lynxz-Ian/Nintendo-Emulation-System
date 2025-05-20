#include <iostream>
#include <memory>
#include "../Header Files/Bus.h"
#include "../Header Files/Cartridge.h"

int main()
{
    // Load the ROM
    auto cart = std::make_shared<Cartridge>("nestest.nes");
    if (!cart || !cart->mapper) {
        std::cerr << "Failed to load ROM or unsupported mapper." << std::endl;
        return 1;
    }

    Bus nes;
    nes.insertCartridge(cart);

    nes.cpu.reset();

    // Run a limited number of CPU cycles for debugging
    for (int i = 0; i < 100; ++i) {
        nes.cpu.clock();
        // Print CPU state for debugging
        std::cout << "PC: $" << std::hex << nes.cpu.pc
                  << "  A: $" << (int)nes.cpu.a
                  << "  X: $" << (int)nes.cpu.x
                  << "  Y: $" << (int)nes.cpu.y
                  << "  P: $" << (int)nes.cpu.status
                  << "  SP: $" << (int)nes.cpu.stkp
                  << std::endl;
    }

    return 0;
}