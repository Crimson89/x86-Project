ECE 486/586 Project - March 16, 2018
		PDP-11 CPU Sumulator

Ethan Grinnell, Daniel Christiansen, Julian Saunders, and Max Schweitzer


How to run the simulator:
1) Compile simulator with "make remake" to clean the working directory, and make the simulator executable.

2) Execute program with "./RunMe.exe -f ./testing/branch/branch.ascii -m verbose -t memory_trace.txt -b trace_branch.txt"
	Program command line options are:
	-f: The ascii program file to load. This can be an absolute or relative path to the file.
	-m: Running verbosity mode, 3 options are available: simple, verbose, and debug
		simple - Prints only required text output, no debug or extended information. This includes error output.
		verbose - includes everything that simple does, but also includes a printout of all valid memory that has
					program sections loading in it and a printout of the register contents. Also included is a
					summary of each instruction as it is run, with a parsed and formatted display of the operation
					
					-------------------------------------------------------------------------
					Instr fetch from address @000306: 000403
					Instruction: BR 000003
					Current Operation PSW:4
					Print Register Contents
					R0:000000
					R1:000000
					R1:000000
					R2:000004
					R3:100000
					R4:000000
					R5:000000
					SP:177776
					PC:000310
					End of Registers
					Branch Trace: BR 000003 from PC 000306 to 000316, branch taken.
					-------------------------------------------------------------------------
		debug - Includes all output from verbose, but also includes memory references and select debug outputs from various instructions
	-t: The file name that the memory trace will be written to. File is opened in append mode, but can have its contents cleared from inside the program.
	-b: Optional, file name to write branch trace to, including this option enables branch tracing. An example branch trace is shown below:

					Branch Trace: BEQ 000003 from PC 000012 to 000022, branch taken.
					Branch Trace: BNE 000003 from PC 000026 to 000036, branch taken.
					Branch Trace: BMI 000003 from PC 000040 to 000050, branch taken.
					Branch Trace: BPL 000003 from PC 000052 to 000062, branch taken.
					Branch Trace: BCS 000003 from PC 000070 to 000100, branch taken.
					Branch Trace: BCC 000003 from PC 000102 to 000112, branch taken.
					Branch Trace: BVS 000003 from PC 000122 to 000132, branch taken.
					Branch Trace: BVC 000003 from PC 000134 to 000144, branch taken.
					Branch Trace: BLT 000003 from PC 000146 to 000156, branch taken.
					Branch Trace: BGE 000003 from PC 000166 to 000176, branch taken.
					Branch Trace: BLE 000003 from PC 000210 to 000220, branch taken.
					
3) The program starts by displaying the system menu. The command line option to enable branch tracing includes two additional menu entries. The rest of the menu items are static.
	All menu selections are case insensitive, either capital or lower case letters can be entered.

					-------------------------------------------------------------------------
					MENU: Make selection from choices below
					Q(q) to quit
					R(r) to Restart current application at initial PC
					S(s) to manually set PC, (Will not automatically start program)
					E(e) to start application at current PC
					P(p) to print all register contents
					M(m) to print all valid memory contents
					L(l) to load new application
					T(t) to print current memory access trace file
					Y(y) to clear old memory access trace file
					B(b) to set break point
					C(c) to clear breakpoint
					V(v) to clear all breakpoints
					N(n) to print all breakpoints
					Z(z) to toggle "breakpoints print register contents" option
					X(x) to toggle "breakpoints print all memory contents" option
					F(f) to print branch trace file
					D(d) to clear old branch trace file
					-------------------------------------------------------------------------
					
	The menu options are:
	Q - Quit the simulator
	R - Restart loaded program from PC value that was either loaded by the file or manually entered by user.
		This does not clear any registers or alter memory. It only changes the PC value.
	S - Allow manual adjustment to the PC. The user is presented with a prompt to enter 6 octal digits for the new program counter.
	E - Execute the loaded program from the current PC. This will generate an error if selected with no program loaded.
	P - Prints the contents of all registers
	M - Print the contents of memory. The program file loader also marks memory as "valid" when the program is read from the file
		into memory. This valid flag is used to display only valid memory contents.
	L - Load an application into the simulator from the ascii file provided from the command line.
	T - Print the current memory reference trace file. The file name is supplied at the command line.
	Y - Yank the contents of the memory reference trace file, clear the file contents. Cannot be undone.
	B - Allows the user to enter an octal address to set a breakpoint at. See the descriptions of Z and X to change breakpoint behavior
    When the program reaches this breakpoint execution will be suspended and debug information is printed.
		Breakpoints will not be triggered for data memory addresses, only instruction fetch triggers breakpoints. Breakpoints for data addresses are ignored. An example breakpoint
		is provided below:
		
				-------------------------------------------------------------------------
				Hit breakpoint @ADDR=000000
				Operation Name: 'MOV', raw op-code value :012701
				Parsed Operation with Operands :MOV (PC)+, R1
				Processor Status Word :
				Global PSW: 000
				Global PSW.SPL: 0
				Global PSW.T: 0
				Global PSW.N: 0
				Global PSW.Z: 0
				Global PSW.V: 0
				Global PSW.C: 0
				-------------------------------------------------------------------------

	C - Allows the user to clear a specific breakpoint by providing the octal address of the breakpoint to clear.
	V - Clears all breakpoints. This cannot be undone.
	N - Lists all breakpoints currently active.
	Z - Toggles debug print behavior when a breakpoint is reached. Enabling this option includes a printout of all register content. This option defaults to false.
	X - Toggles debug print behavior when a breakpoint is reached. Enabling this option includes a printout of all valid memory content. This option defaults to false.
	F - Optional: This is present only if the command line option is included to perform branch tracing. This will print the contents of the current branch trace file.
	D - Optional: This is present only if the command line option is included to perform branch tracing. This will delete the entire contents of the current branch trace file.
		This operation cannot be undone.
	
4) If a breakpoint is reached during execution the user will be provided with the option of continuing the normal program execution, or advancing the program a single step.
	C - Continues normal execution
	S - Enters/Continues single program step mode. The program will only execute fetch and decode the next instruction before triggering a following breakpoint.
		At this point the user can make the same selection, continue normal execution, or continue stepping.
	
		
				-------------------------------------------------------------------------
				
				
						Press C(c) to continue  normal execution, or S(s) to enter "Single Instruction Stepping" mode
	
5) When the program reaches completion and a "HALT" instruction is encountered a summary of the number of instructions is printed.
	This is also included when the program encounters a fault and execution is terminated. Press enter to return to the system menu
	
				-------------------------------------------------------------------------
											Program Completed!
										Executed 46 instructions.
									Press ENTER to return to menu
				-------------------------------------------------------------------------
	
6) To exit the simulator press Q from the system menu and press enter to exit.

				-------------------------------------------------------------------------
				-------------------------------------------------------------------------
				
				
				Selection: q
				-------------------------------------------------------------------------
				-------------------------------------------------------------------------
										Press ENTER to exit
				-------------------------------------------------------------------------
				-------------------------------------------------------------------------



