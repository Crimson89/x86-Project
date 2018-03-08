#include "header.h"

// General
int CLR(instruction *inst) // Clear (B)
{
  uint16_t v_dest = 0;
  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);
  inst->N = 0;
  inst->Z = 1;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

int COM(instruction *inst) // 1's Compliment (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  v_dest = ~v_dest;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = 0;
  inst->C = 1;
  return 0;
}

int INC(instruction *inst) // Increment (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t temp = v_dest;
  v_dest++;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = ((temp == 0xFFFF) && (v_dest == 0x0000))? 1:0;
  inst->C = 0;
  return 0;
}

int DEC(instruction *inst) // Decrement (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t temp = v_dest;
  v_dest--;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = ((temp == 0x0000) && (v_dest == 0xFFFF))? 1:0;
  inst->C = 0;
  return 0;
}

int NEG(instruction *inst) // 2's Compliment negate (B)
{
}

int TST(instruction *inst) // Test (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

// Shift & Rotate
int ASR(instruction *inst) // Arithmetic shift right (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  inst->C = v_dest & 1; // C = old LSB
  v_dest >>= 1;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ASL(instruction *inst) // Arithmetic shift left (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  inst->C = v_dest >> (sizeof(v_dest)*8-1) & 1; // C = old MSB
  v_dest <<= 1;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROR(instruction *inst) // Rotate right (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t temp2 = v_dest;
  uint16_t temp = 0x0001 & v_dest;
  temp <<= 15;
  (v_dest >>= 1) | temp;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->C = (v_dest > temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROL(instruction *inst) // Rotate left (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t temp2 = v_dest;
  uint16_t temp = 0x8000 & v_dest;
  temp >>= 15;
  (v_dest <<= 1) | temp;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->C = (v_dest < temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int SWAB(instruction *inst) // Swap bytes
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t temp = 0x00FF & v_dest;
  temp <<= 8;
  (v_dest >>= 8) | (temp);

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->C = 0;
  inst->V = 0;
  return 0;
}

// Multiple Precision
int ADC(instruction *inst) // Add carry (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t temp = v_dest;
  v_dest += inst->C;

   if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->C = ((temp == 0xFFFF) && (v_dest == 0x0000))? 1:0;
  return 0;
}

int SBC(instruction *inst) // Subtract carry (B)
{
  uint16_t v_dest = get_value(inst->byteMode, inst->dest);
  uint16_t temp = v_dest;
  v_dest -= inst->C;

  if(inst->byteMode)
    write_byte(inst->addressMode, inst->dest, v_dest);
  else
    write_word(inst-a>addressMode, inst->dest, v_dest);

  inst->N = (v_dest < 0)? 1:0;
  inst->Z = (v_dest == 0)? 1:0;
  inst->C = ((temp == 0x0000) && (v_dest == 0xFFFF))? 1:0;
  return 0;
}

int SXT(instruction *inst) // Sign extend
{
}
