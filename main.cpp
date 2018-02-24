#include "header.h"
#define SILENT_MODE 1
#define VERBOSE_MODE 2
#define MEMORY_SPACE ( (1<<(15)) - (1<<(12) ))
#define HALT 0x000000
#define R0_REG_INDEX 0
#define R1_REG_INDEX 1
#define R2_REG_INDEX 2
#define R3_REG_INDEX 3
#define R4_REG_INDEX 4
#define R5_REG_INDEX 5
#define SP_REG_INDEX 6
#define PC_REG_INDEX 7

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
uint16_t starting_pc = 0xFFFF;

/*...*/
int main(int argc, char ** argv)
{
	uint16_t operation;
	PC = 0xFFFF; // Assign at start to invalid value
	SP = 0xFFFF; // Assign at start to invalid value
	ProgramStart:
	if(menu_function() == 1) // The only non-zero return from this function invokes loadData, doesn't return to exit program	
		loadData(argc, argv);
	if(PC != 0xFFFF) {
		while(1) {
			/*...*/
			//do some stuff
			/*...*/
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "                            Executing program" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "-------------------------------------------------------------------------" <<endl;
			cout << "Assigning registers R0 to R6 with data from memory locations MEM[0] to MEM[6]" <<endl;
			REGS[0] = MEM[0];
			REGS[1] = MEM[1];
			REGS[2] = MEM[2];
			REGS[3] = MEM[3];
			REGS[4] = MEM[4];
			REGS[5] = MEM[5];
			REGS[6] = MEM[6];
			cout << "All registers' content:" <<endl;
			cout << "R0=";
			cout<<setfill('0')<<setw(6)<<oct<< R0<<endl;
			cout << "R1=";
			cout<<setfill('0')<<setw(6)<<oct<< R1<<endl;
			cout << "R2=";
			cout<<setfill('0')<<setw(6)<<oct<< R2<<endl;
			cout << "R3=";
			cout<<setfill('0')<<setw(6)<<oct<< R3<<endl;
			cout << "R4=";
			cout<<setfill('0')<<setw(6)<<oct<< R4<<endl;
			cout << "R5=";
			cout<<setfill('0')<<setw(6)<<oct<< R5<<endl;
			cout << "SP=";
			cout<<setfill('0')<<setw(6)<<oct<< SP<<endl;
			cout << "PC=";
			cout<<setfill('0')<<setw(6)<<oct<< PC<<endl;
			cout << "All valid memory contents:" <<endl;
			for(int i = 0; i < MEMORY_SPACE; i++) {
				if(MEM[i]!=0xFFFF) {
					cout <<"@ADDR ";
					print_address_at_index(i);
					cout<<":";
					print_operation(MEM[i]);
					cout<<endl;
				}
			}
			operation = HALT;
			if(operation == HALT)
				break;
		}
	}
	cout << "-------------------------------------------------------------------------" <<endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cout << "                            Program Completed!" <<endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	cout << "-------------------------------------------------------------------------" <<endl;
	goto ProgramStart;
	return 0;
}


uint16_t string_to_operation(string this_operation){
	return (uint16_t(stoul(this_operation, NULL, 8)));
}
	
uint16_t string_to_address(string this_address){
	return (uint16_t(stoul(this_address, NULL, 8))/2); //divided by 2 because memory is x16 and this is a byte addressed ISA
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
		cout << "R(r) to restart current application" << endl;
		cout << "S(s) to set PC in current application and run" << endl;
		cout << "L(l) to load new application" << endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "-------------------------------------------------------------------------" <<endl;
		cout << "\n\nSelection: ";
		cin >> input;
		input_char = tolower(input[0]);
		if ( (input_char != 'l') && (input_char != 'q') && (PC == 0xFFFF) ){
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "  \n\nMust load program into memory first before attempting this operation" << endl;
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
				case 'r':
					cout << "               Press ENTER to restart current application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.get();
					PC = starting_pc;
					cout << "                        Restored PC to " << PC << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					menu_continue = 0;
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
					menu_continue = 0;
					break;
				case 'l':
					cout << "                Press ENTER to load new application" << endl;
					cout << "-------------------------------------------------------------------------" <<endl;
					cin.get();
					return 1;
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
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose]" << endl;
			cerr << "           Press ENTER to exit and close application" << endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cerr << "-------------------------------------------------------------------------" <<endl;
			cin.get();	//Wait for ENTER key press
			exit(EXIT_FAILURE);
		}
	}
	if (mode == -1) { //Print error and return
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "Usage: " << argv[0] << " [-f dataFileName] [-m simple|verbose]" << endl;
		cerr << "             Press ENTER to exit and close application" << endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cin.get();	//Wait for ENTER key press
		exit(EXIT_FAILURE);
	}
	return_val = readData(fileName);
	return return_val;
}


int readData(string fileName){
	string temp;							//File read temp string
	string read_word;						//Store read_word octal characters
	int stringLength = 0;					//Length of temp string
	char code = 'z';						//The type of thing on this line, options are *, @, -
	int address = 0;						//The address of this read_word in MEM
	int file_address = 0;					//Line number in the file being read
	uint16_t temporary_addr = 0;			//Used if this word turns out to be a memory address
	temp.clear();							//initialize temp
	read_word.clear();						//initialize read_word
	ifstream fileInput(fileName);			//File ifstream handle, and open file

	for(int i = 0; i < MEMORY_SPACE; i++) { //Initialize memory space
		MEM[i]=0xFFFF;
	}
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
				MEM[address] = string_to_operation(read_word); //Read string and convert uint16
				cout << "Loading instr: \"";
				print_operation(MEM[address]);
				cout <<"\" into memory"<<endl;
				address+=1;
			}
			else {
				temporary_addr = string_to_address(read_word);
				if (code == '*') {
					starting_pc = temporary_addr;
					PC = temporary_addr;
					cout << "Set PC to start @ address: ";
					print_address(PC);
					cout <<endl;
				}
				else if (code == '@') {
					address = temporary_addr;
					cout << "Moved memory start to address: ";
					print_address(address);
					cout << endl;
				}
				else
					cerr << "Skipping invalid input in file: " << read_word << " at line#" << file_address << endl;
			}
		}
		else
			cerr << "Skipping empty line in file: " << temp << " at line#" << file_address << endl;
		//Clear vars for next pass
		file_address++;
		temp.clear();						
		read_word.clear();
		code = 0;
	}
	fileInput.close(); //done reading, close the file
	cout << "Done reading file, printing valid memory contents:" <<endl;
	for(int i = 0; i < MEMORY_SPACE; i++) {
		if(MEM[i]!=0xFFFF) {
			cout <<"@ADDR ";
			print_address_at_index(i);
			cout <<":";
			print_operation(MEM[i]);
			cout<< endl;
		}
	}
	return 0;
}


