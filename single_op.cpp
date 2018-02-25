#include "header.h"

// General
int CLR(instruction *inst) // Clear (B)
{
}

int COM(instruction *inst) // 1's Compliment (B)
{
}

int INC(instruction *inst) // Increment (B)
{
}

int DEC(instruction *inst) // Decrement (B)
{
}

int NEG(instruction *inst) // 2's Compliment negate (B)
{
}

int NOP(instruction *inst) // No operation
{
}

int TST(instruction *inst) // Test (B)
{
}

int TSTSET(instruction *inst) // Test, set low bit
{
}

int WRTLCK(instruction *inst) // Read/Lock destination, write/unlock R0 into destination
{
}

// Shift & Rotate
int ASR(instruction *inst) // Arithmetic shift right (B)
{
}

int ASL(instruction *inst) // Arithmetic shift left (B)
{
}

int ROR(instruction *inst) // Rotate right (B)
{
}

int ROL(instruction *inst) // Rotate left (B)
{
}

int SWAB(instruction *inst) // Swap bytes
{
}

// Multiple Precision
int ADC(instruction *inst) // Add carry (B)
{
}

int SBC(instruction *inst) // Subtract carry (B)
{
}

int SXT(instruction *inst) // Sign extend
{
}
