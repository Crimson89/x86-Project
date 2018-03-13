#include "header.h"

// General
int CLR(instruction *inst) // Clear (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = 0;
  uint16_t dummyValue = get_value(inst->addressingModeReg, inst->regBase);
  cout << "DESTADDR: " << destAddress << "\n";
  if(inst->byteMode)
  {
    inst->op_text = "CLRB";
    write_byte(inst->addressingModeReg, destAddress, dest);
  }
  else
  {
    inst->op_text = "CLR";
    write_word(inst->addressingModeReg, destAddress, dest);
  }
  inst->N = 0;
  inst->Z = 1;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

int COM(instruction *inst) // 1's Compliment (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  dest = ~dest;

  if(inst->byteMode)
  {
    inst->op_text = "COMB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    inst->op_text = "COM";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = 0;
  inst->C = 1;
  return 0;
}

int INC(instruction *inst) // Increment (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  uint16_t temp = dest;
  dest++;

  if(inst->byteMode)
  {
    inst->op_text = "INCB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
    inst->V = ((temp == 0xFF) && (dest == 0x00))? 1:0;
  }
  else
  {
    inst->op_text = "INC";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
    inst->V = ((temp == 0xFFFF) && (dest == 0x0000))? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = 0;
  return 0;
}

int DEC(instruction *inst) // Decrement (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  uint16_t temp = dest;
  dest--;

  if(inst->byteMode)
  {
    inst->op_text = "DECB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
    inst->V = ((temp == 0x00) && (dest == 0xFF))? 1:0;
  }
   else
   {
    inst->op_text = "DEC";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
    inst->V = ((temp == 0x0000) && (dest == 0xFFFF))? 1:0;
   }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = 0;
  return 0;
}

int NEG(instruction *inst) // 2's Compliment negate (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  inst->V = (dest == 0x8000) ? 1:0;
  dest = (~dest)+1;

  if(inst->byteMode)
  {
    inst->op_text = "NEGB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    inst->op_text = "NEG";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = (dest != 0) ? 1:0;
  return 0;
}

int TST(instruction *inst) // Test (B)
{
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase); 
  if(inst->byteMode)
  {
    inst->op_text = "TSTB";
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    inst->op_text = "TST";
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }
  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = 0;
  inst->C = 0;
  return 0;
}

// Shift & Rotate
int ASR(instruction *inst) // Arithmetic shift right (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  uint16_t msb = dest & 0100000;
  inst->C = dest & 1; // C = old LSB
  dest >>= 1;
  dest |= msb;

   if(inst->byteMode)
  {
    inst->op_text = "ASRB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    inst->op_text = "ASR";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ASL(instruction *inst) // Arithmetic shift left (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  inst->C = dest >> (sizeof(dest)*8-1) & 1; // C = old MSB
  dest <<= 1;

  if(inst->byteMode)
  {
    inst->op_text = "ASLB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    inst->op_text = "ASL";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROR(instruction *inst) // Rotate right (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest; 
  uint16_t temp2;
  uint16_t temp;
  dest = get_value(inst->addressingModeReg, inst->regBase);
  temp = 0x0001 & dest;
  
  if(inst->byteMode)
  { 
    temp <<= 7;
    dest = (dest >> 1);
    dest |= temp;
    inst->op_text = "RORB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    temp <<= 15;
    dest = (dest >> 1);
    dest |= temp;
    inst->op_text = "ROR";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = (dest > temp2)? 1:0;
  inst->V = inst->N ^ inst->C;
  return 0;
}

int ROL(instruction *inst) // Rotate left (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  uint16_t temp2 = dest;
  uint16_t temp;

  cout  << "VAL: "<< dest << "\n";
  if(inst->byteMode)
  { 
    temp = 0x0080 & dest;
    temp >>= 7;
    dest = (dest << 1);
    dest |= temp;
    inst->op_text = "ROLB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    temp = 0x8000 & dest;
    temp >>= 15;
    dest = (dest << 1);
    dest |= temp;
    inst->op_text = "ROL";
    write_word(inst->addressingModeReg, destAddress, dest);
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
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  uint16_t temp = 0x00FF & dest;
  temp <<= 8;
  dest = (dest >> 8); 
  dest |= (temp);

  write_word(inst->addressingModeReg, destAddress, dest);

  inst->N = IS_NEGATIVE_WORD(dest)? 1:0;
  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = 0;
  inst->V = 0;
  return 0;
}

// Multiple Precision
int ADC(instruction *inst) // Add carry (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  uint16_t temp = dest;
  dest += inst->C;

  if(inst->byteMode)
  {
    inst->op_text = "ADCB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    inst->op_text = "ADC";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = ((temp == 0xFFFF) && (dest == 0x0000))? 1:0;
  return 0;
}

int SBC(instruction *inst) // Subtract carry (B)
{
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dest = get_value(inst->addressingModeReg, inst->regBase);
  uint16_t temp = dest;
  dest -= inst->C;

  if(inst->byteMode)
  {
    inst->op_text = "SBCB";
    write_byte(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_BYTE(dest) ? 1:0;
  }
  else
  {
    inst->op_text = "SBC";
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->N = IS_NEGATIVE_WORD(dest) ? 1:0;
  }

  inst->Z = IS_ZERO(dest)? 1:0;
  inst->C = ((temp == 0x0000) && (dest == 0xFFFF))? 1:0;
  return 0;
}

int SXT(instruction *inst) // Sign extend
{
  inst->op_text = "SXT";
  uint16_t destAddress = get_address(inst->addressingModeReg, inst->regBase);
  uint16_t dummyValue = get_value(inst->addressingModeReg, inst->regBase);
  if (inst->N == 1)
  {
    uint16_t dest = 0xFFFF;
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->Z = 0;
  }
  else
  {
    uint16_t dest = 0x0000;
    write_word(inst->addressingModeReg, destAddress, dest);
    inst->Z = 1;
  }
  return 0;
}
