#include "../Header Files/Cartridge.h"
#include "../Header Files/Mapper_000.h"
#include "../Header Files/Mapper_001.h"
#include "../Header Files/Mapper_002.h"
#include "../Header Files/Mapper_003.h"
#include "../Header Files/Mapper_004.h"
#include "../Header Files/Mapper_007.h"
#include "../Header Files/Mapper_066.h"
#include "../Header Files/Mapper_071.h"
#include <iostream>
#include <fstream>

Cartridge::Cartridge(const std::string& sFileName)
{
    // iNES Header
    struct sHeader
    {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);
    if (ifs.is_open())
    {
        // Read file header
        ifs.read((char*)&header, sizeof(sHeader));
        if (header.mapper1 & 0x04)
            ifs.seekg(512, std::ios_base::cur);

        // Determine Mapper ID
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

        // "Discover" File Format
        uint8_t nFileType = 1;
        if (nFileType == 1)
        {
            nPRGBanks = header.prg_rom_chunks;
            vPRGMemory.resize(nPRGBanks * 16384);
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

            nCHRBanks = header.chr_rom_chunks;
            vCHRMemory.resize(nCHRBanks * 8192);
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
        }
        ifs.close();
    }

    // After reading header and setting nMapperID, nPRGBanks, nCHRBanks:
    switch (nMapperID) {
        case 0: mapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks); break;
        case 1: mapper = std::make_shared<Mapper_001>(nPRGBanks, nCHRBanks); break;
        case 2: mapper = std::make_shared<Mapper_002>(nPRGBanks, nCHRBanks); break;
        case 3: mapper = std::make_shared<Mapper_003>(nPRGBanks, nCHRBanks); break;
        case 4: mapper = std::make_shared<Mapper_004>(nPRGBanks, nCHRBanks); break;
        case 7: mapper = std::make_shared<Mapper_007>(nPRGBanks, nCHRBanks); break;
        case 66: mapper = std::make_shared<Mapper_066>(nPRGBanks, nCHRBanks); break;
        case 71: mapper = std::make_shared<Mapper_071>(nPRGBanks, nCHRBanks); break;
        default: mapper = nullptr; break;
    }
}

Cartridge::~Cartridge() {}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data) {
    // Implementation here
    return false; // or your actual logic
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
    // Implement write logic here if needed
    return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
    return false;
}