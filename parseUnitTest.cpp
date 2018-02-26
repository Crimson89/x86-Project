int parseTest()
{

  // Create array of an instruction of every unique type,
  // then iterate through each of them. (94 instructions?)
  int numInstructions = 94;
  int error = 0;
  uint16_t instructionArray[numInstructions] = {000573, 010573, 0005173, 0105173, 0005273}; // Oh boy, this is gonna be fun. Stopped at INCB

  instruction newInstruction;

  //TODO maybe split these up by operand type?
  // also finish the fucker
  for (int i = 0; i < numInstructions; i++)
  {
    error = parseInstruction(instructionArray[i], &newInstruction);
    
    // Print each field out.
    // Source will always be (01), destination will always be (02), register will always be (03),
    // modes will always be (src:04 , dest:05), offset will always be (0066), individual mode is (07).
  }

}
