#include "header.h"


struct {      // Global Processor Status Word
  int SC: 3;
  int N: 1;
  int Z: 1;
  int V: 1;
  int C: 1;
} PSW;

uint16_t REGS[7];                  //CPU Registers
uint8_t MEM[MEMORY_SPACE];         //Byte memory array, index is memory address
bool MEM_USED_FLAGS[MEMORY_SPACE]; //Memory used array, true==memory used/valid, false==invalid
bool BREAK_POINT[MEMORY_SPACE/2];    //Breakpoint address, when PC == this value the program pauses
uint16_t& R0 = REGS[0];
uint16_t& R1 = REGS[1];
uint16_t& R2 = REGS[2];
uint16_t& R3 = REGS[3];
uint16_t& R4 = REGS[4];
uint16_t& R5 = REGS[5];
uint16_t& R6 = REGS[6];
uint16_t& R7 = REGS[7];
uint16_t& SP = REGS[6];
uint16_t& PC = REGS[7];
uint16_t starting_pc;
instruction * current_instruction;	// decoded instruction information
int verbosity_level;                // Level of verbosity in print statements
string trace_file;
string data_file;

int main(int argc, char ** argv)
{
  current_instruction = new instruction;
	//int test;
	//test = parseTest();
	verbosity_level = 0; // Level of verbosity in print statements, default to low 
	int program_execution_control = 0;
	int err;							// error checking
	uint16_t instruction_code;			// 16-bit instruction
	bool at_breakpoint = false;         // Current PC triggered breakpoint
	uint16_t breakpoint_pc;             // PC when breakpoint was triggered
	

	trace_file = "test_trace.txt";
	data_file = "FALSE";
	SP = 0xFFFF; // Assign at start to invalid value, detection of unassigned SP
	PC = 0xFFFF; // Assign at start to invalid value, detection of unassigned PC
	starting_pc = PC;
	initializeMemory();
	get_cmd_options(argc, argv); // Read command line options
	
	for(;;){
		program_execution_control = menu_function();
		if(program_execution_control == LOAD_DATA) {
			if(readData()) {
				cerr << "failed to open file : " << data_file << endl;
			}
			program_execution_control = PRINT_MENU;
		}
		else if((program_execution_control == RUN_PROGRAM) && (PC != 0xFFFF)) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "                     Press ENTER to begin program" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			cout << "                            Executing program" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;

			// Main program loop
			while(program_execution_control == RUN_PROGRAM) {

				// IF
				instruction_code = read_word(MEMORY_READ, PC, READ_TRACE, READ_INSTR_FETCH);
				
//////////////////////////
// Debug, print stuff////
				//cerr << "\n\n Debug: OpCode: " << instruction_code << "\n\n" <<endl;
// End Debug, print stuff////
//////////////////////////
				
				
				PC += 2;
				if(check_breakpoint(PC)){ // Check for a breakpoint pointing to this memory location
					breakpoint_pc = PC;
					at_breakpoint = true;
				}
				else
					at_breakpoint = false;
				

				// ID
				err = parseInstruction(instruction_code, current_instruction); 
				// check error code
				
				// Print breakpoint information
				if(at_breakpoint){
					handle_breakpoint(breakpoint_pc, instruction_code);
					at_breakpoint = false;
				}

				// EX
				err = dispatch(current_instruction);

//////////////////////////
// Debug, print stuff////
//printInstruction(current_instruction);
cout << "Instruction: " << op_formatted(current_instruction) << endl;
// End Debug, print stuff////
//////////////////////////

				// WB
				// updateTracefile();

				if(current_instruction->opcode == m_HALT)
					program_execution_control = PRINT_MENU;
				
				clearInstruction(current_instruction);
			}
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "                            Program Completed!" <<endl;
			cout << "                     Press ENTER to return to menu" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
		}
		else {
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "                  Unhandled condition, is PC set to valid address?!" <<endl;
			cout << "                     Press ENTER to return to menu" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			program_execution_control = PRINT_MENU;
		}
	}
	return 0; // never hit, program exits from menu function
}

void get_user_octal(string prompt, string error_text, uint16_t &word){
	string input = "FALSE";
	string temp = "FALSE";
	while(input == "FALSE") {
		cout << prompt;
		cin >> input;
		for(int i = 0; i < input.length(); i++) {            //Iterate through read line and copy out characters that are valid octal
			if (isxdigit(input[i]) && ((input[i]-48) < 8)) { //Valid octal character
				if(temp.compare("FALSE") == 0)               //If this is the first valid character
					temp = input[i];
				else
					temp += input[i];
			}
		}
		if( temp.compare("FALSE") != 0 && (temp.length() == 6) ) {
			word = string_to_octal(temp);
		}
		else {
			cerr << error_text << endl;
			input = "FALSE";
			temp  = "FALSE";
		}
	}
}


int menu_function() {
	string input;
	char input_char;
	int menu_continue = 1;
	uint16_t bp_address;
	while(menu_continue) {
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "MENU: Make selection from choices below" << endl;
		cout << "Q(q) to quit" << endl;
		cout << "R(r) to Restart current application at initial PC" << endl;
		cout << "S(s) to manually set PC, (Will not automatically start program)" << endl;
		cout << "E(e) to start application at current PC" << endl;
		cout << "P(p) to print all register contents" << endl;
		cout << "M(m) to print all valid memory contents" << endl;
		cout << "L(l) to load new application" << endl;
		cout << "T(t) to print current trace file" << endl;
		cout << "Y(y) to clear old trace file" << endl;
		cout << "B(b) to set break point" << endl;
		cout << "C(c) to clear breakpoint" << endl;
		cout << "V(v) to clear all breakpoints" << endl;
		cout << "N(n) to print all breakpoints" << endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "\n\nSelection: ";
		cin >> input;
		input_char = tolower(input[0]);
		if ( ( (input_char == 'e') || (input_char == 'r') ) && (PC == 0xFFFF) ){
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "  \n\nMust load program into memory first before attempting to execute a program" << endl;
				cout << "                     Press ENTER to continue" << endl;
				cout << "-------------------------------------------------------------------------" <<endl;
				cin.get();
		}
		else {
			switch(input_char){
				case 'q':
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                          Press ENTER to exit" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					exit(EXIT_SUCCESS);
					break;
				case 't':
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Print trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					print_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'y':
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Delete old trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					clear_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'p':
					cout << "All registers' content:" <<endl;
					print_all_registers();
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'm':
					cout << "All valid memory contents:" <<endl;
					print_all_memory();
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'r':
					cout << "                  Restarting current application" << endl;
					PC = starting_pc;
					cout << "                        Set PC to " << PC << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					return RUN_PROGRAM;
					break;
				case 'e':
					cout << "                        Starting application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					return RUN_PROGRAM;
					break;
				case 'b':
					get_user_octal("Input octal breakpoint address(6 digits), then press ENTER:",
					                   "ERROR: Invalid input\n\nPlease input address (6 octal digits), then press ENTER:",
									   bp_address);
					set_breakpoint(bp_address);
					cout <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'c':
					get_user_octal("Input octal breakpoint address to clear (6 digits), then press ENTER:",
					                   "ERROR: Invalid input\n\nPlease input address (6 octal digits), then press ENTER:",
									   bp_address);
					clear_breakpoint(bp_address);
					cout <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'v':
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Clearing all breakpoints" << endl;
					clear_all_breakpoints();
					cout <<endl;
					cout << "                      Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'n':
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Printing all breakpoints" << endl;
					print_all_breakpoints();
					cout <<endl;
					cout << "                      Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 's':
					get_user_octal("Input octal address to load into PC (6 digits), then press ENTER:",
					                   "ERROR: Invalid input\n\nPlease input address to load into PC (6 octal digits), then press ENTER:",
									   PC);
					cout << "Set PC to start @ address: ";
					print_octal(PC);
					cout <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'l':
					cout << "                Press ENTER to load new application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.get();
					return LOAD_DATA;
					break;
				default:
					cout << "                     ERROR: Invalid input\n\n" << endl;
					break;
			}
		}
	}
	return 0;
}
