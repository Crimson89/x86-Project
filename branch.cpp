#include "header.h"

// Branch
int BR(instruction *inst) // Branch unconditional
{
}

int BNE(instruction *inst) // Branch if not equal (to zero)
{
}

int BEQ(instruction *inst) // Branch if equal (to zero)
{
}
int BPL(instruction *inst) // Branch if plus
{
}

int BMI(instruction *inst) // Branch if minus
{
}

int BVC(instruction *inst) // Branch if overflow is clear
{
}

int BVS(instruction *inst) // Branch if overflow is set
{
}

int BCC(instruction *inst) // Branch if carry is clear
{
}

int BCS(instruction *inst) // Branch if carry is set
{
}

// Signed Conditional Branch
int BGE(instruction *inst) // Branch if greater than or equal (to zero)
{
}

int BLT(instruction *inst) // Branch if less than (zero)
{
}

int BGT(instruction *inst) // Branch if greater than (zero)
{
}

int BLE(instruction *inst) // Branch if less than or equal (to zero)
{
}

int SOB(instruction *inst) // Subtract one and branch (if not = 0)
{
}

// Unsigned Conditional Branch
int BHI(instruction *inst) // Branch if higher
{
}

int BLOS(instruction *inst) // Branch if lower or same
{
}

int BHIS(instruction *inst) // Branch if higher or same
{
}

int BLO(instruction *inst) // Branch if lower
{
}
