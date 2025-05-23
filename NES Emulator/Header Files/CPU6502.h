#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>

class Bus;
class CPU6502
{
public:
    CPU6502();
    ~CPU6502();
    
public:
    bool complete() const { return cycles == 0; }

     enum FLAGS6502
    {
        C = (1 << 0), // Carry Bit
        Z = (1 << 1), // Zero Bit
        I = (1 << 2), // Interrupt Disable
        D = (1 << 3), // Decimal Mode
        B = (1 << 4), // Break
        U = (1 << 5), // Unused
        V = (1 << 6), // Overflow
        N = (1 << 7)  // Negative
    };

    uint8_t a       = 0x00;       // Accumulator  
    uint8_t x       = 0x00;       // X Register
    uint8_t y       = 0x00;       // Y Register
    uint8_t stkp    = 0x00;       // Stack Pointer
    uint16_t pc     = 0x0000;     // Program Counter
    uint16_t status = 0x0;        // Status Pointer

    void ConnectBus(Bus *n) { bus = n; }
    std::map<uint16_t, std::string> disassemble(uint16_t start, uint16_t stop);
        uint16_t line_addr = 0x0000;
    
    void clock();
    void reset();
    void irq();
    void nmi();
    
private:
    // Addressing Modes
    uint8_t IMP(); uint8_t IMM(); uint8_t ZP0(); uint8_t ZPX(); uint8_t ZPY();
    uint8_t REL(); uint8_t ABS(); uint8_t ABX(); uint8_t ABY();
    uint8_t IND(); uint8_t IZX(); uint8_t IZY();

    // Instructions
    uint8_t ADC(); uint8_t AND(); uint8_t ASL(); uint8_t BCC(); uint8_t BCS();
    uint8_t BEQ(); uint8_t BIT(); uint8_t BMI(); uint8_t BNE(); uint8_t BPL();
    uint8_t BRK(); uint8_t BVC(); uint8_t BVS(); uint8_t CLC(); uint8_t CLD();
    uint8_t CLI(); uint8_t CLV(); uint8_t CMP(); uint8_t CPX(); uint8_t CPY();
    uint8_t DEC(); uint8_t DEX(); uint8_t DEY(); uint8_t EOR(); uint8_t INC();
    uint8_t INX(); uint8_t INY(); uint8_t JMP(); uint8_t JSR(); uint8_t LDA();
    uint8_t LDX(); uint8_t LDY(); uint8_t LSR(); uint8_t NOP(); uint8_t ORA();
    uint8_t PHA(); uint8_t PHP(); uint8_t PLA(); uint8_t PLP(); uint8_t ROL();
    uint8_t ROR(); uint8_t RTI(); uint8_t RTS(); uint8_t SBC(); uint8_t SEC();
    uint8_t SED(); uint8_t SEI(); uint8_t STA(); uint8_t STX(); uint8_t STY();
    uint8_t TAX(); uint8_t TAY(); uint8_t TSX(); uint8_t TXA(); uint8_t TXS();
    uint8_t TYA();

    // Illegal/undocumented
    uint8_t XXX();

    // Unofficial/Undocumented Opcodes (examples)
    uint8_t LAX(); // LDA + LDX
    uint8_t SAX(); // Store A & X
    uint8_t DCP(); // DEC + CMP
    uint8_t ISC(); // INC + SBC
    uint8_t SLO(); // ASL + ORA
    uint8_t RLA(); // ROL + AND
    uint8_t SRE(); // LSR + EOR
    uint8_t RRA(); // ROR + ADC
    uint8_t ANC();
    uint8_t ALR();
    uint8_t ARR();
    uint8_t XAA();
    uint8_t XAS();
    uint8_t AXS();
    // ...add more as needed
  
    uint8_t fetch();
    uint8_t fetched = 0x00;     // Temporary variable to hold fetched data

    uint16_t addr_abs = 0x0000; // Absolute address
    uint16_t addr_rel = 0x0000; // Relative address
    uint8_t opcode    = 0x00;   // Current opcode
    uint8_t cycles    = 0;      // Number of cycles to execute

    





private:
    struct INSTRUCTION
    {
        std::string name;
        uint8_t(CPU6502::*operate)(void) = nullptr;
        uint8_t(CPU6502::*addrmode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> lookup;
    Bus *bus = nullptr;
    uint8_t read(uint16_t a);
    void    write(uint16_t a, uint8_t d);

    // Functions to access the status register
    uint8_t GetFlag(FLAGS6502 f);
    void    SetFlag(FLAGS6502 f, bool v);

};

// Example for LAX (opcode 0xA7, 0xB7, 0xAF, 0xBF, 0xA3, 0xB3)
// { "LAX", &a::LAX, &a::ZP0, 3 }, // 0xA7
