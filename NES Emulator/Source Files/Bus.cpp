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
