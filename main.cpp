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
uint16_t operation;
/*...*/
int main(int argc, char ** argv)
{
	ProgramStart:
	//menu_function();
	loadData(argc, argv, MEM, REGS);
	printf("Press ENTER");
	cin.get();
	while(1) {
		/*...*/
		//do some stuff
		/*...*/
		REGS[0] = MEM[0];
		REGS[1] = MEM[1];
		REGS[2] = MEM[2];
		REGS[3] = MEM[3];
		REGS[4] = MEM[4];
		REGS[5] = MEM[5];
		REGS[6] = MEM[6];
		cout << "In main, printing non-zero memory" <<endl;
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
		for(int i = 0; i < MEMORY_SPACE; i++) {
		 if(MEM[i]) {
			 cout << "@"<<i<<":";
			 cout<<setfill('0')<<setw(6)<<oct<< MEM[i]<<endl;
		 }
		}
		operation = HALT;
		if(operation == HALT)
			break;
	}
	//goto ProgramStart;
	return 0;
}

int loadData(int argc, char ** argv, uint16_t MEM[], uint16_t REGS[]){
	int mode = -1;
	int return_val = 1;
	string fileName = "./loadData.dat"; //Default file name of data file
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
			cerr << "				 Press ENTER to exit and close application" << endl;
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
		cerr << "				 Press ENTER to exit and close application" << endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cerr << "-------------------------------------------------------------------------" <<endl;
		cin.get();	//Wait for ENTER key press
		exit(EXIT_FAILURE);
	}
	return_val = readData(fileName, MEM, REGS);
	cerr << "-------------------------------------------------------------------------" <<endl;
	cerr << "-------------------------------------------------------------------------" <<endl;
	cerr << "												Closing application" << endl;
	cerr << "-------------------------------------------------------------------------" <<endl;
	cerr << "-------------------------------------------------------------------------" <<endl;
	return return_val;
}


int readData(string fileName, uint16_t MEM[], uint16_t REGS[]){
	string temp;									 //File read temp string
	int stringLength = 0;					//Length of temp string
	char code;										 //The type of thing on this line, options are *, @, -
	string instruction;						//Store instruction octal characters
	int address = 0;							 //The address of this instruction
	uint16_t memory_word;
	temp.clear();									//initialize temp
	instruction.clear();					 //initialize instruction
	ifstream fileInput(fileName);

	for(int i = 0; i < MEMORY_SPACE; i++) {
		MEM[i]=0;
	}
	if(!fileInput.is_open()) {
		cerr << "failed to open file : " << fileName << endl;
		return 1;
	}
	while(getline(fileInput, temp))// continue reading consecutive lines until EOF is reached
	{
		stringLength = temp.length();
		if( stringLength > 1) {
			code = temp[0];
			for(int i = 1; i < stringLength; i++) { //Iterate through read line and copy out characters that are valid octal
				if (isxdigit(temp[i]) && ((temp[i]-48) < 8))	//Valid octal character
					instruction += temp[i];
			}
			if(instruction.length() == 6 && code == '-') {
				memory_word = uint16_t(stoul(instruction, NULL, 8)); //Read string and convert uint16
				cout << "Loading instr=" << instruction << endl;
				MEM[address] = memory_word;
			}
			else if (code == '*') {
				cout << "Set PC to start @=" << instruction << endl;
				REGS[PC_REG_INDEX] = uint16_t(stoul(instruction, NULL, 8));
			}
			else if (code == '@') {
				cout << "Memory Starts @=" << instruction << endl;
				address = (uint16_t(stoul(instruction, NULL, 8))/2);
			}
			else
				cout << "Skipping invalid input=" << instruction << endl;
		}
		else
			cerr << "Invalid instruction " << temp << " at address " << address <<endl;
		//Clear vars for next pass
		address+=1;
		temp.clear();						
		instruction.clear();
		code = 0;
	}
	fileInput.close(); //done reading, close the file
	cout << "Done reading, printing non-zero memory" <<endl;
	for(int i = 0; i < MEMORY_SPACE; i++) {
		if(MEM[i]) {
			cout << "@"<<i<<":";
			cout<<setfill('0')<<setw(6)<<oct<< MEM[i]<<endl;
		}
	}
	return 0;
}


