#include "header.h"

/*

Takes a strng octal argument and converts ut to an unsigned long long integer
Returns the integer value

*/
uint16_t string_to_octal(string this_string){
	return stoul(this_string, NULL, 8);
}

string octal_to_string(uint16_t value) {
	stringstream temp;
	temp << setfill('0') << setw(6) << oct << value;
	return temp.str();
}

/*

Reads a single byte from the memory array
Arguments:
addressMode - 	0 is a register read, data is read from the register with the index provided as the address.
				Otherwise this is a memory read that is read from the memory array at the supplied address
address			Any uint16 address is allowed, determines location to read from
trace			Should this read be included in the trace file? True traces the read, false prevents tracing

Returns the data byte read from memory
*/
uint16_t read_byte(uint16_t addressMode, uint16_t address, bool trace) {
	uint16_t data;
  if (addressMode == 0) //Register mode
  {
	data = REGS[address] & 0x00FF; // Just reade the low register byte
  }
  else   				//Memory mode
  {
    data = 0x00FF&MEM[address];
	if(trace)					//If trace is enabled
		read_trace(address, data);
  }
	return (data);
}

/*

Reads a 16 bit word from the memory array
Arguments:
addressMode - 	0 is a register read, data is read from the register with the index provided as the address.
				Otherwise this is a memory read that is read from the memory array at the supplied address
address			Any uint16 address is allowed, determines location to read from
trace			Should this read be included in the trace file? True traces the read, false prevents tracing
is_instruction	Enabling this changes the trace mode from data trace to instruction trace. Setting to false treats this as a data read.

Returns the data word read from memory

*/
uint16_t read_word(uint16_t addressMode, uint16_t address, bool trace, bool is_instruction) {
	uint16_t data;
  
  if (addressMode == 0) //Register mode
  {
	data = REGS[address];
  }
  else   				//Memory mode
  {
    data = ((MEM[address+1]<<8) | (MEM[address]));
	if(trace)					//If trace is enabled
		read_trace(address, data, is_instruction);
  }
	return (data);
}

/*

Writes a single byte to the memory array
Arguments:
addressMode - 	0 is a register write, data is written to the register with the index provided as the address.
				Otherwise this is a memory write that is written to the memory array at the supplied address
address			Any uint16 address is allowed, determines location to write to
trace			Should this write be included in the trace file? True traces the write, false prevents tracing

Return: None

*/
void write_byte(uint16_t addressMode, uint16_t address, uint16_t byte, bool trace) {
	uint16_t tempValue = 0;

  if (addressMode == 0) //Register mode
  {
    tempValue = 0xFF00 & REGS[address];
    tempValue = tempValue | (byte & 0x00FF);
    REGS[address] = tempValue;
  }
  else   				//Memory mode
  {
    if(trace)					//If trace is enabled
		write_trace(address, byte);
	  MEM[address] = 0x00FF&byte;
	  MEM_USED_FLAGS[address] = true;
  }
}

/*

Writes a 16 bit word from the memory array

Arguments:
addressMode - 	0 is a register write, data is written to the register with the index provided as the address.
				Otherwise this is a memory write that is written to the memory array at the supplied address
address			Any uint16 address is allowed, determines location to write to
trace			Should this write be included in the trace file? True traces the write, false prevents tracing

Return: None


*/
void write_word(uint16_t addressMode, uint16_t address, uint16_t word, bool trace) {
	
  if (addressMode == 0) //Register mode
  {
    REGS[address] = word;
  }
  else   				//Memory mode
  {
    if(trace)					//If trace is enabled
		write_trace(address, word);
	MEM[address]   = (0x00FF&word);
	MEM_USED_FLAGS[address] = true;
	MEM[address+1] = ((0xFF00&word)>>8);
	MEM_USED_FLAGS[address+1] = true;
  }
}

/*

Prints the uint16_t supplied as an argument in octal to stdout

Arguments: None
Return: None


*/
void print_octal(uint16_t value){
	cout <<setfill('0')<<setw(6)<<oct<<value;
}

/*

Prints all valid memory locations to stdout. Memory is printed in words. Both address and data are given in octal

Arguments: None
Return: None

*/
void print_all_memory(void) {
	uint16_t hasContent = 0;
	uint16_t memory_word;
	for(int i = 0; i < (MEMORY_SPACE); i+=2) { // Increment by word, this is a word access
		if(MEM_USED_FLAGS[i] == true) {        // If this memory location is valid
			memory_word = read_word(MANUAL_MEMORY_READ, i, NO_TRACE, READ_NOT_INSTR_FETCH);
			hasContent+=1;                     // Increment word counter
			cout <<"@ADDR="; print_octal(i); cout <<", Contents=" << octal_to_string(memory_word);
			if( i == PC )                      // If this memory location is the address pointed to by the PC
				cout << " <-- PC";
			if( i == SP )                      // If this memory location is the address pointed to by the SP
				cout << " <-- SP";
			cout<< endl;
		}
	}
	if(hasContent) {  							//Print the number of words read
		cout << "Memory contains " <<dec<<hasContent*2 << " bytes of information" << endl;
	}
	else
		cout << "Memory is empty" << endl;
}

/*

Sets all memory locations to 0 and clears memory used/valid flags

Arguments: None
Return: None

*/
void initializeMemory(){
	for(int i = 0; i < (MEMORY_SPACE); i++) { //Initialize memory space, by byte, since this is a byte access
		write_byte(MANUAL_MEMORY_WRITE, i,0x00, NO_TRACE);
		MEM_USED_FLAGS[i] = false;
	}
}

/*

Prints all register content to stdout. Register data is given in octal

Arguments: None
Return: None

*/
void print_all_registers(void) {
	for(int i = 0; i < 6; i++) { // Print registers 0 - 5
		cout <<"R"<<i<<":";
		print_octal(REGS[i]);
		cout<< endl;
	}
	cout <<"SP"<<":";           // Print SP contents
	print_octal(SP);
	cout<< endl;
	cout <<"PC"<<":";          // Print PC contents
	print_octal(PC);
	if(PC == 0xFFFE)           // PC is not permitted to be an odd address, so it is initialized to an odd address to indicate that it has not been setup
		cout<< "  <-- This is an invalid PC, 0xFFFE";
	cout<< endl;
}

/*

Parses the command line arguments and sets the data file, trace file, and optional branch trace file names.
Also sets the verbosity level

Arguments: 
			argc - integer number of supplied command line arguments
			argv - char * array of command line argument strings
			branch_trace_file - Passed by reference string used to provide the optional branch trace file name	
			data_file         - Passed by reference string used to provide the program ascii file name	
			trace_file        - Passed by reference string used to provide the memory trace file name
Return: Integer status code, 0 indicates no issue. If an problem is encountered this function does not return dirctly, instead exit() is called

*/
int get_cmd_options(int argc, char ** argv, string & branch_trace_file, string & data_file, string & trace_file){
	int opt;
	optind = 0;			//force getOpt to restart for repeated loading from file
	verbosity_level = -1;
	trace_file = "FALSE";
	data_file = "FALSE";
	while ((opt = getopt(argc, argv, "f:m:t:b:")) != -1) {  // Parse the command line options
		switch (opt) {
		case 'f':
			data_file = string(optarg);
			break;
		case 't':
			trace_file = string(optarg);
			break;
		case 'b':
			branch_trace_file = string(optarg);
			break;
		case 'm':
			if (string(optarg) == "simple")
				verbosity_level = LOW_VERBOSITY;
			else if (string(optarg) == "verbose")
				verbosity_level = HIGH_VERBOSITY;
			else if (string(optarg) == "debug")
				verbosity_level = DEBUG_VERBOSITY;
			else	 //Default to invalid mode
				verbosity_level = -1;
			break;
		case 'h':   //-h prints help menu and exits success
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose] [-t traceFileName] optional: ([-b branchTraceFile])" << endl;
			cout << "                All three arguments are mandatory" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			exit(EXIT_SUCCESS);
			break;
		default:   // Not all required options are provided
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "Error, invalid option" << endl;
			cout << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose] [-t traceFileName] optional: ([-b branchTraceFile])" << endl;
			cerr << "           Press ENTER to exit and close application" << endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cin.get();	//Wait for ENTER key press
			exit(EXIT_FAILURE);
		}
	}
	if (verbosity_level <= -1) { //Print error and return
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "ERROR: Invalid mode:" << endl;
		cout << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose] [-t traceFileName] optional: ([-b branchTraceFile])" << endl;
		cerr << "             Press ENTER to exit and close application" << endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cin.get();	//Wait for ENTER key press
		exit(EXIT_FAILURE);
	}
	if(trace_file == "FALSE") { // No trace file name supplied
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "ERROR: Did not specify trace file:" << endl;
		cout << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose] [-t traceFileName] optional: ([-b branchTraceFile])" << endl;
		cerr << "             Press ENTER to exit and close application" << endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cin.get();	//Wait for ENTER key press
		exit(EXIT_FAILURE);
	}
	if(data_file == "FALSE") {  //  No data file name supplied
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "ERROR: Did not specify data input file name:" << endl;
		cout << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose] [-t traceFileName] optional: ([-b branchTraceFile])" << endl;
		cerr << "             Press ENTER to exit and close application" << endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cin.get();	//Wait for ENTER key press
		exit(EXIT_FAILURE);
	}
	cout << "Data File " << data_file << endl;
	cout << "Trace File " << trace_file << endl;
	if(branch_trace_file != "FALSE") // If branch tracing is enabled
		cout << "Branch Trace File " << branch_trace_file << endl;
	cout << "Verbosity set to " << verbosity_level << endl;
	return 0;
}

/*

Reads the program data ascii file and loads it into memory

Arguments: 
			data_file         - Passed by reference string used to provide the program ascii file name	
Return: Integer status code, 0 indicates no issue. Non-zero return indicates reading failed

*/
int readData(string & data_file){
	string temp;							//File read temp string
	string input_word;						//Store input_word octal characters
	int stringLength = 0;					//Length of temp string
	char line_type = 'z';					//The type of thing on this line, valid options are '*', '@', and '-'
	int address = 0;					    //The address of this input_word in MEM
	int file_line_num = 0;					//Line number in the file being read
	uint16_t temporary_addr = 0;			//Used if this word turns out to be a memory address
	temp.clear();							//initialize temp
	input_word.clear();						//initialize input_word
	ifstream fileInput(data_file);			//File ifstream handle, and open file

	initializeMemory();
	if(!fileInput.is_open()) {              // Return error if file could not be opened
		return 1;
	}
	cout << "Reading from:" << data_file << "\n";
	while(getline(fileInput, temp))			// continue reading consecutive lines until EOF is reached
	{
		stringLength = temp.length();
		if( stringLength > 1) {
			line_type = temp[0];
			if(verbosity_level > HIGH_VERBOSITY) cout << "Reading data on line #"<<file_line_num<<",\t\""<<temp<<"\""<<endl;
			if(verbosity_level >= DEBUG_VERBOSITY) cout << "Line type "<<line_type<<endl;
			for(int i = 1; i < stringLength; i++) { 			//Iterate through read line and copy out characters that are valid octal
				if (isxdigit(temp[i]) && ((temp[i]-48) < 8))	//Valid octal character
					input_word += temp[i];
				else  //If invalid input received, break out of for loop, stop parsing this line, move to next line in file
					break;
			}
			if(verbosity_level >= DEBUG_VERBOSITY) cout << "input_word is "<<input_word<<endl;
			if(input_word.length() == 6 && line_type == '-') {    //If this line started with character '-' and the length beyond that character is 6 characters
				if(verbosity_level >= DEBUG_VERBOSITY) cout << "Memory load"<<endl;
				if(verbosity_level >= DEBUG_VERBOSITY) cout << "Loading:  " << input_word<<endl;
				if(verbosity_level >= DEBUG_VERBOSITY) cout << "@address: " << octal_to_string(address)<<endl;
				write_word(FILE_WRITE, address,string_to_octal(input_word), NO_TRACE); //Read string, convert uint16, and write to memory
				if(verbosity_level >= DEBUG_VERBOSITY) cout << "Loaded to memory, Read Back: " << octal_to_string(read_word(FILE_READ, address, NO_TRACE, READ_NOT_INSTR_FETCH))<<endl;
				address+=2;
			}
			else { //Otherwise, is this a memory index, or is it a PC value?
				temporary_addr = string_to_octal(input_word);
				if (line_type == '*') {  // PC value
					starting_pc = temporary_addr;
					PC = starting_pc;
					if(verbosity_level >= DEBUG_VERBOSITY) cout << "Set PC to start @ address: " << octal_to_string(PC) <<endl;
				}
				else if (line_type == '@') { // Memory location, adjust memory index
					address = temporary_addr;
				if(verbosity_level == DEBUG_VERBOSITY) cout << "Set current memory location to address: " << octal_to_string(address) << endl;
				}
				else				// Skip invalid lines
					cerr << "Skipping invalid input in file: " << input_word << " at line#" <<dec<< file_line_num << endl;
			}
		}
		else
			cerr << "Skipping empty line in file: " << temp << " at line#" <<dec<< file_line_num << endl;
		//Clear and set vars for next line in file
		file_line_num++;
		temp.clear();						
		input_word.clear();
		line_type = 'z';
		stringLength = 0;
		temporary_addr = 0;
	}
	fileInput.close(); //done reading, close the file
	cout << "Done reading file";
	if(verbosity_level == HIGH_VERBOSITY) { cout << ", printing valid memory contents: " << endl; print_all_memory();}
	cout <<endl;
	if (PC == 0xFFFE) { //See if PC was set, this is an invalid PC, can never be an odd number. Prompt for new PC value
		cerr << "\n\n-------------------------------------------------------------------------" <<endl;
		cerr << "Error: PC not set"<< endl;
		get_user_octal("Enter octal starting PC value: ", "ERROR: Enter only 6 octal digits: ", PC);
		starting_pc= PC;
		cerr << "Set PC to start @ address: " << octal_to_string(PC) <<endl;
		cerr << "                     Press ENTER to continue" << endl;
		cerr << "-------------------------------------------------------------------------\n\n" <<endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.get();
	}
	cout << "-------------------------------------------------------------------------" <<endl;
	cout << "                     Loading memory completed" << endl;
	cout << "                     Press ENTER to continue" << endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cin.get();
	return 0;
}


