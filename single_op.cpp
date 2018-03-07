#include "header.h"

// General
int CLR(instruction *inst) // Clear (B)
{
  inst->dest = 0;
  inst->N = 0;
  inst->Z = 1;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

int COM(instruction *inst) // 1's Compliment (B)
{
  inst->dest = ~inst->dest;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = 0;
  inst->C = 1;
  return 0;
}

int INC(instruction *inst) // Increment (B)
{
  uint16_t temp = inst->dest;
  inst->dest++;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = ((temp == 0xFFFF) && (inst->dest == 0x0000))? 1:0;
  inst->C = 0;
  return 0;
}

int DEC(instruction *inst) // Decrement (B)
{
  uint16_t temp = inst->dest;
  inst->dest--;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = ((temp == 0x0000) && (inst->dest == 0xFFFF))? 1:0;
  inst->C = 0;
  return 0;
}

int NEG(instruction *inst) // 2's Compliment negate (B)
{
}

int TST(instruction *inst) // Test (B)
{
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

// Shift & Rotate
int ASR(instruction *inst) // Arithmetic shift right (B)
{
  inst->C = inst->dest & 1; // C = old LSB
  inst->dest >>= 1;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ASL(instruction *inst) // Arithmetic shift left (B)
{
  inst->C = inst->dest >> (sizeof(inst->dest)*8-1) & 1; // C = old MSB
  inst->dest <<= 1;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROR(instruction *inst) // Rotate right (B)
{
  uint16_t temp2 = inst->dest;
  uint16_t temp = 0x0001 & inst->dest;
  temp <<= 15;
  (inst->dest >>= 1) | temp;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->C = (inst->dest > temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROL(instruction *inst) // Rotate left (B)
{
  uint16_t temp2 = inst->dest;
  uint16_t temp = 0x8000 & inst->dest;
  temp >>= 15;
  (inst->dest <<= 1) | temp;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->C = (inst->dest < temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int SWAB(instruction *inst) // Swap bytes
{
  uint16_t temp = 0x00FF & inst->dest;
  temp <<= 8;
  (inst->dest >>= 8) | (temp);
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->C = 0;
  inst->V = 0;
  return 0;
}

// Multiple Precision
int ADC(instruction *inst) // Add carry (B)
{
  uint16_t temp = inst->dest;
  inst->dest += inst->C;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->C = ((temp == 0xFFFF) && (inst->dest == 0x0000))? 1:0;
  return 0;
}

int SBC(instruction *inst) // Subtract carry (B)
{
  uint16_t temp = inst->dest;
  inst->dest -= inst->C;
  inst->N = (inst->dest < 0)? 1:0;
  inst->Z = (inst->dest == 0)? 1:0;
  inst->C = ((temp == 0x0000) && (inst->dest == 0xFFFF))? 1:0;
  return 0;
}

int SXT(instruction *inst) // Sign extend
{
}
