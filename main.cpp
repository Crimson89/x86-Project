#include "header.h"

//Make REGS and MEM global
uint16_t REGS[7];
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
uint16_t MEM[MEMORY_SPACE];
uint16_t starting_pc;
instruction current_instruction;	// decoded instruction information

int main(int argc, char ** argv)
{
  //int test;
  //test = parseTest();
  
	uint16_t operation;
	int program_execution_control = 0;
	SP = 0xFFFF; // Assign at start to invalid value, detection of unassigned SP
	PC = 0xFFFF; // Assign at start to invalid value, detection of unassigned PC
	starting_pc = PC;
	initializeMemory();
	int err;							// error checking
	uint16_t instruction_code;			// 16-bit instruction
	
	for(;;){
		program_execution_control = menu_function();
		if(program_execution_control == LOAD_DATA) {
			loadData(argc, argv);
			program_execution_control = PRINT_MENU;
		}
		else if((program_execution_control == RUN_PROGRAM) && (PC != 0xFFFF)) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "                     Press ENTER to begin program" << endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cin.get();
			cout << "                            Executing program" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "Assigning registers R0 to R5 with data from memory locations MEM[2] to MEM[7]" <<endl;
			R0 = MEM[2];
			R1 = MEM[3];
			R2 = MEM[4];
			R3 = MEM[5];
			R4 = MEM[6];
			R5 = MEM[7];
			cout << "Set SP to memory index 18 (Octal address 000044)" <<endl;
			SP = index_to_address(18);
	
			// Main program loop
			while(program_execution_control == RUN_PROGRAM) {
				/*
				cout << "All registers' content:" <<endl;
				print_all_registers();
				cout << "All valid memory contents:" <<endl;
				print_all_memory();
				operation = m_HALT;
				*/

				// IF
				instruction_code = *((uint16_t*)(&MEM[PC]));
				PC += 2;	

				// ID
				err = parseInstruction(instruction_code, &current_instruction); 
				// check error code
				err = loadOperands();	// deal w/ addressing modes
				// pre-increment
				// updateTracefile();

				// EX
				// call appropriate function				

				// WB
				// post-increment
				// updateTracefile();
				

				if(operation == m_HALT)
					program_execution_control = PRINT_MENU;
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

int storeOperands() {

}

int loadOperands() {

}


uint16_t string_to_operation(string this_operation){
	return stoul(this_operation, NULL, 8);
}
	
uint16_t string_to_address(string this_address){
	return stoul(this_address, NULL, 8);
}

uint16_t address_to_index(uint16_t this_address){
	return (this_address/2); //divided by 2 because memory array is x16 and this is a byte addressed ISA
}

uint16_t index_to_address(uint16_t this_index){
	return (this_index*2); //Multiplied by 2 because memory array is x16 and this is a byte addressed ISA
}

void print_operation(uint16_t operation){
	cout <<setfill('0')<<setw(6)<<oct<<operation;
}

void print_address(uint16_t address){
	print_operation(address);
}

void print_address_at_index(int index){
	cout <<setfill('0')<<setw(6)<<oct<<(index*2);
}

void print_all_memory(void) {
	uint16_t hasContent = 0;
	for(uint16_t i = 0; i < MEMORY_SPACE; i++) {
		if(MEM[i]!=0xFFFF) {
			hasContent+=1;
			cout <<dec<<"@Index:\t"<<i;
			cout <<"\tADDR="; print_address_at_index(i); cout <<", Contents="; print_operation(MEM[i]);
			if(PC != 0xFFFF && (i == address_to_index(PC)))
				cout << " <-- PC";
			if(SP != 0xFFFF && (i == address_to_index(SP)))
				cout << " <-- SP";
			cout<< endl;
		}
	}
	if(hasContent)
		cout << "Memory contains " <<dec<<(hasContent*2) << " bytes of information" << endl;
	else
		cout << "Memory is empty" << endl;
}


void initializeMemory(){
	for(int i = 0; i < MEMORY_SPACE; i++) { //Initialize memory space
		MEM[i]=0xFFFF; // Why?
	}
}

void print_all_registers(void) {
	for(int i = 0; i < 5; i++) {
		cout <<"R"<<i<<":";
		print_operation(REGS[i]);
		cout<< endl;
	}
	cout <<"SP"<<":";
	print_operation(SP);
	if(SP == 0xFFFF)
		cout<< "  <-- This is an invalid SP, 0xFFFF";
	cout<< endl;
	cout <<"PC"<<":";
	print_operation(PC);
	if(PC == 0xFFFF)
		cout<< "  <-- This is an invalid PC, 0xFFFF";
	cout<< endl;
}

int menu_function() {
	string input;
	string temp_pc;
	char input_char;
	int menu_continue = 1;
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
					cin.get();
					exit(EXIT_SUCCESS);
					break;
				case 'p':
					cout << "All registers' content:" <<endl;
					print_all_registers();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.get();
					break;
				case 'm':
					cout << "All valid memory contents:" <<endl;
					print_all_memory();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "-------------------------------------------------------------------------" <<endl;
					cout << "                     Press ENTER to continue" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
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
				case 's':
					cout << "        Press ENTER to manually set PC in current application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					input = "FALSE";
					while(input == "FALSE") {
						temp_pc = "FALSE";
						cout << "Input octal address to load into PC (6 digits), then press ENTER:";
						cin >> input;
						for(int i = 0; i < input.length(); i++) { //Iterate through read line and copy out characters that are valid octal
							if (isxdigit(input[i]) && ((input[i]-48) < 8)) {	//Valid octal character
								if(temp_pc.compare("FALSE") == 0) //If this is the first valid character
									temp_pc = input[i];
								else
									temp_pc += input[i];
							}
						}
						if( temp_pc.compare("FALSE") != 0 && (temp_pc.length() == 6) ) {
							PC = string_to_address(temp_pc);
							cout << "Set PC to start @ address: ";
							print_address(PC);
							cout <<endl;
							cout << "                          Press ENTER to continue" << endl;
							cout << "-------------------------------------------------------------------------" <<endl;
							cin.get();
						}
						else {
							cerr << "ERROR: Invalid input\n\nPlease input address to load into PC (6 octal digits), then press ENTER:";
							input = "FALSE";
							temp_pc = "FALSE";
						}
					}
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

int loadData(int argc, char ** argv){
	int mode = -1;
	int return_val = 1;
	string fileName;	//File name of data file
	int opt;
	optind = 0;			//force getOpt to restart for repeated loading from file
	while ((opt = getopt(argc, argv, "f:m:")) != -1) {
		switch (opt) {
		case 'f':
			fileName = string(optarg);
			break;
		case 'm':
			if (string(optarg) == "simple")
				mode = SILENT_MODE;
			else if (string(optarg) == "verbose")
				mode = VERBOSE_MODE;
			else	 //Default to invalid mode
				mode = -1;
			break;
		default:
			cerr << "Hit default case" << endl;
			cerr << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose]" << endl;
			cerr << "           Press ENTER to exit and close application" << endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cin.get();	//Wait for ENTER key press
			exit(EXIT_FAILURE);
		}
	}
	if (mode == -1) { //Print error and return
		cerr << "Invalid mode:" << endl;
		cerr << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose]" << endl;
		cerr << "             Press ENTER to exit and close application" << endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cin.get();	//Wait for ENTER key press
		exit(EXIT_FAILURE);
	}
	cout << "\n" << fileName << "\n";
	return_val = readData(fileName);
	return return_val;
}


int readData(string fileName){
	string temp;							//File read temp string
	string read_word;						//Store read_word octal characters
	int stringLength = 0;					//Length of temp string
	char code = 'z';						//The type of thing on this line, options are *, @, -
	int address_index = 0;					//The address of this read_word in MEM
	int file_address = 0;					//Line number in the file being read
	uint16_t temporary_addr = 0;			//Used if this word turns out to be a memory address
	temp.clear();							//initialize temp
	read_word.clear();						//initialize read_word
	ifstream fileInput(fileName);			//File ifstream handle, and open file

	initializeMemory();
	if(!fileInput.is_open()) {
		cerr << "failed to open file : " << fileName << endl;
		return 1;
	}
	while(getline(fileInput, temp))			// continue reading consecutive lines until EOF is reached
	{
		stringLength = temp.length();
		if( stringLength > 1) {
			code = temp[0];
			for(int i = 1; i < stringLength; i++) { 			//Iterate through read line and copy out characters that are valid octal
				if (isxdigit(temp[i]) && ((temp[i]-48) < 8))	//Valid octal character
					read_word += temp[i];
				else  //If invalid input received, break out of for loop, stop parsing this line
					break;
			}
			if(read_word.length() == 6 && code == '-') {
				MEM[address_index] = string_to_operation(read_word); //Read string and convert uint16
				cout << "Loading instr: \""; print_operation(MEM[address_index]); cout <<"\" into memory"<<endl;
				address_index+=1;
			}
			else {
				temporary_addr = string_to_address(read_word);
				if (code == '*') {
					starting_pc = temporary_addr;
					PC = temporary_addr;
					cout << "Set PC to start @ address: "; print_address(PC); cout <<endl;
				}
				else if (code == '@') {
					address_index = address_to_index(temporary_addr);
					cout << "Set current memory location to address: "; print_address(temporary_addr); cout << endl;
					cout << "Stored at address (Derived from index):" << index_to_address(address_index) <<endl;
					cout << "Stored at index (Derived from address):" <<dec<< address_to_index(temporary_addr)<<endl;
				}
				else
					cerr << "Skipping invalid input in file: " << read_word << " at line#" <<dec<< file_address << endl;
			}
		}
		else
			cerr << "Skipping empty line in file: " << temp << " at line#" <<dec<< file_address << endl;
		//Clear vars for next pass
		file_address++;
		temp.clear();						
		read_word.clear();
		code = 0;
	}
	fileInput.close(); //done reading, close the file
	cout << "Done reading file, printing valid memory contents:" <<endl;
	print_all_memory();
	cout << "-------------------------------------------------------------------------" <<endl;
	cout << "                     Loading memory completed" << endl;
	cout << "                     Press ENTER to continue" << endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cin.get();
	return 0;
}


