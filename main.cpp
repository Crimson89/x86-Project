#include "header.h"

// Define global values
uint16_t REGS[7];                  	//CPU Registers
uint8_t MEM[MEMORY_SPACE];         	//Byte memory array, index is memory address
bool MEM_USED_FLAGS[MEMORY_SPACE]; 	//Memory used array, true==memory used/valid, false==invalid
bool BREAK_POINT[MEMORY_SPACE/2];  	//Breakpoint address, when PC == this value the program pauses
uint16_t& R0 = REGS[0];				//Pointer aliases to register array
uint16_t& R1 = REGS[1];
uint16_t& R2 = REGS[2];
uint16_t& R3 = REGS[3];
uint16_t& R4 = REGS[4];
uint16_t& R5 = REGS[5];
uint16_t& R6 = REGS[6];
uint16_t& R7 = REGS[7];
uint16_t& SP = REGS[6];
uint16_t& PC = REGS[7];
uint16_t starting_pc;				// Place holder for PC value provided from data file or by user. Used to restart probram being simulated from the beginning
instruction * current_instruction;	// decoded instruction information
int verbosity_level;                // Level of verbosity in print statements , 1, 2, or 3 for simple, verbose, and debug level print statements
string trace_file;					// Filename of the memory access trace file
string data_file;                   // Filename of the program data ascii file
string branch_trace_file;           // Optional filename for branch instruction tracing
PSW_t PSW;                          // Global processor status word


/*
Main program

Arguments: 
			argc - integer number of supplied command line arguments
			argv - char * array of command line argument strings

Return: None directly, exit() is called from main menu loop.

*/
int main(int argc, char ** argv)
{
	verbosity_level = 0; // Level of verbosity in print statements, default to low 
	int program_execution_control = 0;
	int err;							// error checking
	uint16_t instruction_code;			// 16-bit instruction
	bool at_breakpoint = false;         // Current PC triggered breakpoint
	bool program_step_mode = false;     // Stepping mode enabled, breakpoint at each new instruction
	uint16_t breakpoint_pc;             // PC when breakpoint was triggered
	uint16_t if_pc_value = 0;           // PC address after instruction fetch
	int num_instructions_executed = 0;  // The number of instructions executed by this program
	bool bp_print_mem = false;  		// Hitting a breakpoint prints all valid memory contents
	bool bp_print_regs = false; 		// Hitting a breakpoint prints all register contents 
	bool branch_trace_en = false;       // Enable branch tracing
	
	//Set initial state
	trace_file = "test_trace.txt";
	data_file = "FALSE";
	branch_trace_file = "FALSE";
	SP = 0xFFFE; // Assign at start to invalid value, detection of unassigned SP
	PC = 0xFFFE; // Assign at start to invalid value, detection of unassigned PC
	starting_pc = PC;
	initializeMemory();
	get_cmd_options(argc, argv, branch_trace_file, data_file, trace_file); // Read command line options
	current_instruction = new instruction;
	clearInstruction(current_instruction);
	clear_psw(PSW);
	
	if(branch_trace_file != "FALSE")  // If a branch trace filename is supplied, enable the branch tracing option
		branch_trace_en = true;
	
	for(;;){						 // Main program loop
		program_execution_control = menu_function(bp_print_mem, bp_print_regs, trace_file, branch_trace_en);	// Display system menu
		if(program_execution_control == LOAD_DATA) {  // If user selected load data from the menu then call readData function
			cout << "Data File " << data_file << endl;
			if(readData(data_file)) {
				cerr << "failed to open file : " << data_file << endl;
			}
			program_execution_control = PRINT_MENU;	// Don't enter simulator loop, reprint menu
		}
		else if((program_execution_control == RUN_PROGRAM) && (PC != 0xFFFE)) { //Program start selected from menu, and PC set to valid location
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "                     Press ENTER to begin program" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			cout << "                            Executing program" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			program_step_mode = false;
			num_instructions_executed = 0;

			// Main simulation loop
			while(program_execution_control == RUN_PROGRAM) { // Continue this loop until program control becomes someething other than run program

				if(verbosity_level >= HIGH_VERBOSITY) cout << "-------------------------------------------------------------------------" <<endl;
				// IF and evaluate current PC for matching breakpoint, then increment PC
				if(instruction_fetch(at_breakpoint, instruction_code, breakpoint_pc, PC, if_pc_value)) {
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n\t\tInstruction Fetch Fault, Terminating Program!!\n\n" << endl;
					cerr << "\t\t\tPress ENTER to return to menu" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_execution_control = PRINT_MENU; // Exit the simulation loop
					program_step_mode = false;
					cin.get();
					break;
				}
				

				// ID
				if(parseInstruction(instruction_code, current_instruction)) {
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n\t\tInstruction Decode Fault, Terminating Program!!\n\n" << endl;
					cerr << "\t\t\tPress ENTER to return to menu" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_execution_control = PRINT_MENU; // Exit the simulation loop
					program_step_mode = false;
					cin.get();
					break;
				}
				
				// If this is a breakpoint, then print breakpoint information
				if(at_breakpoint || program_step_mode){
					program_step_mode = handle_breakpoint(breakpoint_pc, instruction_code, program_step_mode, bp_print_mem, bp_print_regs);
					at_breakpoint = false;
				}
				else if(verbosity_level >= HIGH_VERBOSITY) {
					cout << "Instruction: " << op_formatted(current_instruction) << endl;
					cout << "Current Operation PSW:" << oct << uint16_t(current_instruction->PSW) << endl;
					cout << "Print Register Contents" << endl;
					print_all_registers();
					cout << "End of Registers" << endl;
				}

				// EX
				if(dispatch(current_instruction)){
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n\t\tInstruction Execution/Dispatch Fault\n\n" << endl;
					cerr << "\t\t\tPress ENTER to continue" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_execution_control = PRINT_MENU; // Exit the simulation loop
					program_step_mode = false;
					cin.get();
					break;
				}

				// WB
				if(write_back(PSW, current_instruction)) {
					cerr << "\n\n-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "\n\n   PSW Write Back Fault, Terminating Program!!\n\n" << endl;
					cerr << "              Press ENTER to return to menu" << endl;
					cerr << "-------------------------------------------------------------------------" <<endl;
					cerr << "-------------------------------------------------------------------------\n\n" <<endl;
					program_execution_control = PRINT_MENU; // Exit the simulation loop
					program_step_mode = false;
					cin.get();
				}
				
				//Branch trace, if enabled
				if(current_instruction->is_branch && branch_trace_en){
					if(verbosity_level > HIGH_VERBOSITY) cout << "Tracing branch" << endl;
					branch_trace(if_pc_value, current_instruction->branch_target, op_formatted(current_instruction) , current_instruction->branch_taken);
				}
				if(verbosity_level >= HIGH_VERBOSITY) cout << "-------------------------------------------------------------------------" <<endl;
				
				// If this operation was halt, then exit the simulation loop
				if(current_instruction->opcode == m_HALT) {
					program_step_mode = false;
					program_execution_control = PRINT_MENU;
				}
				
				num_instructions_executed++;
				clearInstruction(current_instruction);
			}	//End of simulation or error occured
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "                            Program Completed!" <<endl;
			cout << "                          Executed " << dec << num_instructions_executed << " instructions." <<endl;
			cout << "                     Press ENTER to return to menu" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
		}
		else { // Unhandled option, print error
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "              Unhandled condition, is PC set to valid address?!" <<endl;
			cerr << "                 Press ENTER to return to menu" << endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			program_execution_control = PRINT_MENU;
			program_step_mode = false;
		}
	}
	delete current_instruction;
	return 0; // never hit, program exits from menu function
}

/*

Gets a 16 bit octal data word supplied by the user
Errors if the user enters non octal characters or doesn't enter 6 characters
Continues loop until valid input received

Arguments:
	prompt:	Message text to the user, including a prompt i.e. ":"
	error_text: Error message to user, does not include prompt
	word: 16 bit data word returned by reference
	
Return: None, result passed by reference

*/
void get_user_octal(string prompt, string error_text, uint16_t &word){
	string input = "FALSE";
	string temp = "FALSE";
	while(input == "FALSE") {                                // Continue until valid input received
		cout << prompt;
		cin >> input;
		for(int i = 0; i < input.length(); i++) {            //Iterate through read line and copy out characters that are valid octal
			if (isxdigit(input[i]) && ((input[i]-48) < 8)) { //Valid octal character
				if(temp.compare("FALSE") == 0)               //If this is the first valid character
					temp = input[i];
				else
					temp += input[i];						//Concatenate into valid octal string
			}
		}
		if( temp.compare("FALSE") != 0 && (temp.length() == 6) ) {	//Valid 6 characters received, convert to uint16 and return
			word = string_to_octal(temp);
		}
		else {														// Invalid input, print error text
			cerr << error_text << endl;
			input = "FALSE";
			temp  = "FALSE";
		}
	}
}

/*

Displays menu to user, takes user input, configures options, and returns program control value

Arguments:
	bp_print_menu - bool by reference indicating whether a breakpoint will include printing valid memory
	bp_print_regs - bool by reference indicating whether a breakpoint will include printing all registers
	trace_file    - string by reference containing the filename of the memory trace file
	branch_trace_file - string by reference containing the filename of the optional branch trace file
	branch_trace_en - bool incicating whether branch tracing is enabld
	
Return: integer program control code:
		PRINT_MENU  0
		RUN_PROGRAM 1
		LOAD_DATA   2

*/
int menu_function(bool & bp_print_mem, bool & bp_print_regs, string & trace_file, bool branch_trace_en) {
	string input;
	char input_char;
	int menu_continue = 1;
	uint16_t bp_address;
	while(menu_continue) {  //Continue menu loop
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
		cout << "T(t) to print current memory access trace file" << endl;
		cout << "Y(y) to clear old memory access trace file" << endl;
		cout << "B(b) to set break point" << endl;
		cout << "C(c) to clear breakpoint" << endl;
		cout << "V(v) to clear all breakpoints" << endl;
		cout << "N(n) to print all breakpoints" << endl;
		cout << "Z(z) to toggle \"breakpoints print register contents\" option" << endl;
		cout << "X(x) to toggle \"breakpoints print all memory contents\" option" << endl;
		if(branch_trace_en) {
			cout << "F(f) to print branch trace file" << endl;
			cout << "D(d) to clear old branch trace file" << endl;
		}
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "\n\nSelection: ";
		cin >> input;
		input_char = tolower(input[0]);
		if ( ( (input_char == 'e') || (input_char == 'r') ) && (PC == 0xFFFE) ){  //Prevent program execution with invalid PC
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "  \n\nMust load program into memory first before attempting to execute a program" << endl;
				cout << "                     Press ENTER to continue" << endl;
				cout << "-------------------------------------------------------------------------" <<endl;
				cin.get();
		}
		else {
			switch(input_char){     //Handle user input
				case 'q':           //Quit program, delete current_instruction memory
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                          Press ENTER to exit" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					delete current_instruction;
					exit(EXIT_SUCCESS);
					break;
				case 'z':   //Toggle breakpoint printing registers
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Toggle breakpoint register print";
					if(bp_print_regs)
						bp_print_regs = false;
					else
						bp_print_regs = true;
					if(!bp_print_regs)
							cout << " to false" << endl;
					else
							cout << " to true" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'x':  //Toggle breakpoint printing memory
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Toggle breakpoint memory print";
					if(bp_print_mem)
						bp_print_mem = false;
					else
						bp_print_mem = true;
					if(!bp_print_mem)
							cout << " to false" << endl;
					else
							cout << " to true" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 't':  //Print memory access trace file
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Printing trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					print_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'y':  //Clear memory access trace file
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Deleting old trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					clear_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'p':  //Print register content
					cout << "All registers' content:" <<endl;
					print_all_registers();
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'm':  //Print valid memory content
					cout << "All valid memory contents:" <<endl;
					print_all_memory();
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'r':  //Restart simulation at previously recorded starting_pc
					cout << "                  Restarting current application" << endl;
					PC = starting_pc;
					cout << "                        Set PC to " << PC << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					return RUN_PROGRAM;
					break;
				case 'e':	// Start simulation program at currently set PC value
					cout << "                        Starting application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					return RUN_PROGRAM;
					break;
				case 'b':	// Enable a breakpoint
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
				case 'c':	// Clear a particular breakpoint
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
				case 'v':	// Clear all breakpoints
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Clearing all breakpoints" << endl;
					clear_all_breakpoints();
					cout <<endl;
					cout << "                      Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'n':	// Print all breakpoint addresses
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Printing all breakpoints" << endl;
					print_all_breakpoints(bp_print_mem, bp_print_regs);
					cout <<endl;
					cout << "                      Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 's':	// Manually specify a PC value
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
				case 'l':	// Call load_data function
					cout << "                Press ENTER to load new application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.get();
					return LOAD_DATA;
					break;
				case 'f':	// Print branch trace file contents
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Printing branch trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					print_branch_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				case 'd':	// Delete branch trace file contents
					cout << "\n\n-------------------------------------------------------------------------" <<endl;
					cout << "                     Deleting old branch trace file, " << trace_file << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					clear_branch_trace();
					cout << "                          Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin.get();
					break;
				default:	// Invalid input, print error
					cerr << "                     ERROR: Invalid input\n\n" << endl;
					break;
			}
		}
	}
	return 0;
}


/*

Fetches an instruction from memory address and increments the PC by 2

Arguments:
	at_breakpoint - bool by reference out of this function indicating that a breakpoint was reached
	instruction_code - uint16_t by reference containing the instruction fetched from memory
	breakpoint_pc - uint16_t by reference indicating the current PC, used later for handling this breakpoint
	PC	-	uint16_t passed by reference of the current PC, included here as an early start at reduing use of global variables
	if_pc_value - uint16_t passed by reference indicating the current PC, used later by branch tracing, if enabled
	
Return: 0 for success, 1 for failure

Generates a failure if an access of an odd memory address is performed

*/
int instruction_fetch(bool & at_breakpoint, uint16_t & instruction_code,  uint16_t & breakpoint_pc, uint16_t & PC, uint16_t & if_pc_value){
	instruction_code = read_word(MEMORY_READ, PC, READ_TRACE, READ_INSTR_FETCH);
	if_pc_value = PC;	
	if(check_breakpoint(PC)){ // Check for a breakpoint pointing to this memory location
		breakpoint_pc = PC;
		at_breakpoint = true;
	}
	else
		at_breakpoint = false;		
	PC += 2;
	if(PC%2 == 1) {//IF at odd memory address not allowed
		cerr << "Error: Instruction fetch from odd address is not permitted, halting program!" <<endl;
		return 1;
	}
	return 0;
}

/*

Clears the global processor status word

Arguments:
	PSW: PSW_t by reference containing the global processor status
	
Return: None

*/
void clear_psw(PSW_t & PSW) {
	PSW.PSW_BYTE = 0;
}

/*

Prints the global processor status word

Arguments:
	PSW: PSW_t by reference containing the global processor status
	
Return: None

*/
void print_psw(PSW_t & PSW) {
	cout << "Global PSW: " << setfill('0') << setw(3) << oct << uint16_t(PSW.PSW_BYTE) << endl;
	cout << "Global PSW.SPL: " << setfill('0') << setw(1) << uint16_t(PSW.SPL) << endl;
	cout << "Global PSW.T: " << setfill('0') << setw(1) << uint16_t(PSW.T) << endl;
	cout << "Global PSW.N: " << setfill('0') << setw(1) << uint16_t(PSW.N) << endl;
	cout << "Global PSW.Z: " << setfill('0') << setw(1) << uint16_t(PSW.Z) << endl;
	cout << "Global PSW.V: " << setfill('0') << setw(1) << uint16_t(PSW.V) << endl;
	cout << "Global PSW.C: " << setfill('0') << setw(1) << uint16_t(PSW.C) << endl;
}

/*
Writes the current instructions temporary processor status word to the global processor status

Arguments:
	PSW: PSW_t by reference containing the global processor status
	inst: instruction data struct containing the current instruction and it's temporary PSW
	
Return: 0 if no error, and 1 if PSW failed to be set correctly and the written value does not match the temporary value
*/
int write_back(PSW_t & PSW, instruction * inst) {
	PSW.PSW_BYTE = inst->PSW;
	if(PSW.PSW_BYTE != inst->PSW) { // I'm not sure how this would happen, but to allow some type of fault return
		cerr << "Failed to update PSW from current operation!" <<endl;
		return 1;
	}
	return 0;
}
