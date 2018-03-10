#include "header.h"

// General
int CLR(instruction *inst) // Clear (B)
{
  uint16_t dest = 0;
  if(inst->byteMode)
    write_byte(inst->addressingModeDest, inst->destBase, dest);
  else
    write_word(inst->addressingModeDest, inst->destBase, dest);
  inst->N = 0;
  inst->Z = 1;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

int COM(instruction *inst) // 1's Compliment (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  dest = ~dest;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->V = 0;
  inst->C = 1;
  return 0;
}

int INC(instruction *inst) // Increment (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest++;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
    inst->V = ((temp == 0xFF) && (dest == 0x00))? 1:0;
  }
   else
   {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
    inst->V = ((temp == 0xFFFF) && (dest == 0x0000))? 1:0;
   }

  inst->Z = (dest == 0)? 1:0;
  inst->C = 0;
  return 0;
}

int DEC(instruction *inst) // Decrement (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest--;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
    inst->V = ((temp == 0x00) && (dest == 0xFF))? 1:0;
  }
   else
   {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
    inst->V = ((temp == 0x0000) && (dest == 0xFFFF))? 1:0;
   }

  inst->Z = (dest == 0)? 1:0;
  inst->C = 0;
  return 0;
}

int NEG(instruction *inst) // 2's Compliment negate (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  inst->V = (dest == 0x8000) ? 1:0;
  dest = (~dest)+1;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->C = (dest != 0) ? 1:0;
  return 0;
}

int TST(instruction *inst) // Test (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  if(inst->byteMode)
    inst->N = (dest && 0x80) ? 1:0;
  else
    inst->N = (dest && 0x8000) ? 1:0;
  inst->Z = (dest == 0)? 1:0;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

// Shift & Rotate
int ASR(instruction *inst) // Arithmetic shift right (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  inst->C = dest & 1; // C = old LSB
  dest >>= 1;

   if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ASL(instruction *inst) // Arithmetic shift left (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  inst->C = dest >> (sizeof(dest)*8-1) & 1; // C = old MSB
  dest <<= 1;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROR(instruction *inst) // Rotate right (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp2 = dest;
  uint16_t temp = 0x0001 & dest;
  temp <<= 15;
  (dest >>= 1) | temp;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->C = (dest > temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROL(instruction *inst) // Rotate left (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp2 = dest;
  uint16_t temp = 0x8000 & dest;
  temp >>= 15;
  (dest <<= 1) | temp;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->C = (dest < temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int SWAB(instruction *inst) // Swap bytes
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = 0x00FF & dest;
  temp <<= 8;
  (dest >>= 8) | (temp);

  write_word(inst->addressingModeDest, inst->destBase, dest);

  inst->N = (dest && 0x8000)? 1:0;
  inst->Z = (dest == 0)? 1:0;
  inst->C = 0;
  inst->V = 0;
  return 0;
}

// Multiple Precision
int ADC(instruction *inst) // Add carry (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest += inst->C;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->C = ((temp == 0xFFFF) && (dest == 0x0000))? 1:0;
  return 0;
}

int SBC(instruction *inst) // Subtract carry (B)
{
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest -= inst->C;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x80) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = (dest && 0x8000) ? 1:0;
  }

  inst->Z = (dest == 0)? 1:0;
  inst->C = ((temp == 0x0000) && (dest == 0xFFFF))? 1:0;
  return 0;
}

int SXT(instruction *inst) // Sign extend
{
  if (inst->N == 1)
  {
    uint16_t dest = 0xFFFF;
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->Z = 0;
  }
  else
  {
    uint16_t dest = 0x0000;
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->Z = 1;
  }
  return 0;
}
