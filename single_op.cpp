#include "header.h"

// General
int CLR(instruction *inst) // Clear (B)
{
  if(inst->byteMode)
    inst->op_text = "CLRB";
  else
    inst->op_text = "CLR";
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
  if(inst->byteMode)
    inst->op_text = "COMB";
  else
    inst->op_text = "COM";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  dest = ~dest;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = 0;
  inst->C = 1;
  return 0;
}

int INC(instruction *inst) // Increment (B)
{
  if(inst->byteMode)
    inst->op_text = "INCB";
  else
    inst->op_text = "INC";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest++;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
    inst->V = ((temp == 0xFF) && (dest == 0x00))? 1:0;
  }
   else
   {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
    inst->V = ((temp == 0xFFFF) && (dest == 0x0000))? 1:0;
   }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = 0;
  return 0;
}

int DEC(instruction *inst) // Decrement (B)
{
  if(inst->byteMode)
    inst->op_text = "DECB";
  else
    inst->op_text = "DEC";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest--;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
    inst->V = ((temp == 0x00) && (dest == 0xFF))? 1:0;
  }
   else
   {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
    inst->V = ((temp == 0x0000) && (dest == 0xFFFF))? 1:0;
   }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = 0;
  return 0;
}

int NEG(instruction *inst) // 2's Compliment negate (B)
{
  if(inst->byteMode)
    inst->op_text = "NEGB";
  else
    inst->op_text = "NEG";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  inst->V = (dest == 0x8000) ? 1:0;
  dest = (~dest)+1;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = (dest != 0) ? 1:0;
  return 0;
}

int TST(instruction *inst) // Test (B)
{
  if(inst->byteMode)
    inst->op_text = "TSTB";
  else
    inst->op_text = "TST";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  if(inst->byteMode)
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  else
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

// Shift & Rotate
int ASR(instruction *inst) // Arithmetic shift right (B)
{
  if(inst->byteMode)
    inst->op_text = "ASRB";
  else
    inst->op_text = "ASR";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  inst->C = dest & 1; // C = old LSB
  dest >>= 1;

   if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ASL(instruction *inst) // Arithmetic shift left (B)
{
  if(inst->byteMode)
    inst->op_text = "ASLB";
  else
    inst->op_text = "ASL";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  inst->C = dest >> (sizeof(dest)*8-1) & 1; // C = old MSB
  dest <<= 1;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROR(instruction *inst) // Rotate right (B)
{
  if(inst->byteMode)
    inst->op_text = "RORB";
  else
    inst->op_text = "ROR";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp2 = dest;
  uint16_t temp = 0x0001 & dest;
  temp <<= 15;
  (dest >>= 1) | temp;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = (dest > temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROL(instruction *inst) // Rotate left (B)
{
  if(inst->byteMode)
    inst->op_text = "ROLB";
  else
    inst->op_text = "ROL";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp2 = dest;
  uint16_t temp = 0x8000 & dest;
  temp >>= 15;
  (dest <<= 1) | temp;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = (dest < temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int SWAB(instruction *inst) // Swap bytes
{
  inst->op_text = "SWAB";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = 0x00FF & dest;
  temp <<= 8;
  (dest >>= 8) | (temp);

  write_word(inst->addressingModeDest, inst->destBase, dest);

  inst->N = IS_NEGATIVE_WORD(dest)? 1:0;
  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = 0;
  inst->V = 0;
  return 0;
}

// Multiple Precision
int ADC(instruction *inst) // Add carry (B)
{
  if(inst->byteMode)
    inst->op_text = "ADCB";
  else
    inst->op_text = "ADC";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest += inst->C;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = ((temp == 0xFFFF) && (dest == 0x0000))? 1:0;
  return 0;
}

int SBC(instruction *inst) // Subtract carry (B)
{
  if(inst->byteMode)
    inst->op_text = "SUBB";
  else
    inst->op_text = "SUB";
  uint16_t dest = get_value(inst->addressingModeDest, inst->destBase);
  uint16_t temp = dest;
  dest -= inst->C;

  if(inst->byteMode)
  {
    write_byte(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    write_word(inst->addressingModeDest, inst->destBase, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = ((temp == 0x0000) && (dest == 0xFFFF))? 1:0;
  return 0;
}

int SXT(instruction *inst) // Sign extend
{
  inst->op_text = "SXT";
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
