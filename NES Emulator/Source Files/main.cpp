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

    // Run CPU until a certain number of instructions have completed
    int instructions = 100; // or however many you want to step
    while (instructions > 0)
    {
        nes.cpu.clock();
        if (nes.cpu.complete())
        {
            // Print CPU state for debugging
            std::cout << "PC: $" << std::hex << nes.cpu.pc
                      << "  A: $" << (int)nes.cpu.a
                      << "  X: $" << (int)nes.cpu.x
                      << "  Y: $" << (int)nes.cpu.y
                      << "  P: $" << (int)nes.cpu.status
                      << "  SP: $" << (int)nes.cpu.stkp
                      << std::endl;
            instructions--;
        }
    }

    return 0;
}