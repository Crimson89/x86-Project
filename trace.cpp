#include "header.h"

int write_line(const string & file_name, string text) {
	ofstream outfile;
	outfile.open(file_name, ios_base::app);
	if(outfile.is_open()) { //Open/create file and append this line
		outfile << text << endl;
		return 0;
	}
	cerr << "Failed to open file: "<< file_name <<endl;
	return 1;
}

int clear_trace(const string & file_name) {
	ofstream outfile;
	outfile.open(file_name);
	if(outfile.is_open()) //Open/create file and erase contents
		return 0;
	cerr << "Failed to open file: "<< file_name <<endl;
	return 1;
}

int print_trace(const string & file_name) {
	string temp;
	temp.clear();
	cout << "Reading trace file :" << file_name << "\n";
	cout << "\n--- Start of trace file ---\n"<< endl;
	ifstream fileInput(file_name);
	if(!fileInput.is_open()) {
		return 1;
	}
	while(getline(fileInput, temp)) {
		cout << temp<<endl;
		temp.clear();
	}
	cout << "\n--- End of trace file ---\n"<< endl;
	return 0;
}

int data_read_trace(const string & file_name, uint16_t address, uint16_t value) {
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Data read from address   @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
	return write_line(file_name, ("0 "+octal_to_string(address)) );
}

int data_write_trace(const string & file_name, uint16_t address, uint16_t value) {
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Data write to address    @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
	return write_line(file_name, ("1 "+octal_to_string(address)) );
}

int instr_fetch_trace(const string & file_name, uint16_t address, uint16_t value) {
	if(verbosity_level >= HIGH_VERBOSITY) cout << "Instr fetch from address @"<<octal_to_string(address)<<": "<<octal_to_string(value)<<endl;
	return write_line(file_name, ("2 "+octal_to_string(address)) );
}
