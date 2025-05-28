# X8000 Engine

X8000 is an architecture for a CPU emulator. Essentially, the X8000 engine is a virtual machine that runs programs with a specific binary architecture.
Only X8000 compatible programs can be run in this engine.

In this file, I intend to describe the X8000 documentation. This documentation is divided into two parts, the first part is the engine architecture itself and the second part is the X8000 architecture (executable binary programs).

# Engine Architecture

This engine must be able to work at two levels. The first level is the operating system and hardware, this engine must be able to manage the operations related to the operating system and hardware. At the second level is the target program. The target program is the program that is being executed. This engine must be able to execute the entire program correctly and interact with the first level if necessary.

## First Level

The first-level architecture should be modular, such that each section should be available as a packet in memory so that the engine program can use it when needed.

### Available Packets

The available packets are as follows:
- stdio: Input/output related functions.
- stdmem: Functions related to memory management.

### Second Level

At the second level, everything simply needs to be implemented.

# X8000 Architecture

The architecture of X8000 is very simple and convenient. In this architecture we have several registers. These registers are as follows:

- IP: The instruction pointer points to the location in memory that the X8000 engine should execute. This register is incremented by one with each instruction executed.
- RK: This is the kernel register. Using this register, the program can interact with the first level of the engine.
- RC: This is an internal register used for the CMP instruction where a flag is stored that indicates the result of the comparison.
- SP: This is an internal register used save call addresss.
- R1 ... R8: These registers are used in general.
- RP1 ... RP8: These registers are used to set the parameters of functions.
- RR1 ... RR8: These registers are used to return a value from the function.

**note:** All registers are 8-bit.

## Instructions

**Note:** All commands are stored in one byte.

The X8000 instructions are as follows:

|Instruction|Hex|e.g|Description|
|-----------|---|---|-----------|
|`MOV`|`0x20`|`MOV RK, R0`|Used to transfer a register to a register.|
|`MOV`|`0x21`|`MOV RK, 0xFF`|Used to transfer a 1-byte number to a register.|
|`MOV`|`0x22`|`MOV RK, 0xFFFF`|Used to transfer a 2-byte number to a register.|
|`MOV`|`0x23`|`MOV RK, 0xFFFFFFFF`|Used to transfer a 4-byte number to a register.|
|`MOV`|`0x23`|`MOV RK, 0xFFFFFFFFFFFFFFFF`|Used to transfer a 8-byte number to a register.|
|`MOV`|`0x24`|`MOV RK, 0xFFFFFFFFFFFFFFFF`|Used to transfer an address to a register.|
|`CMP`|`0x31`|`CMP R1, R2`|Compares two registers.|
|`CMP`|`0x32`|`CMP R1, 0xFF`|To compare a 1-byte number with a register.|
|`CMP`|`0x33`|`CMP R1, 0xFFFF`|To compare a 2-byte number with a register.|
|`CMP`|`0x34`|`CMP R1, 0xF0xFFFFFFFFF`|To compare a 4-byte number with a register.|
|`CMP`|`0x35`|`CMP R1, 0xFFFFFFFFFFFFFFFF`|To compare a 8-byte number with a register.|
|`JMP`|`0x40`|`JMP 0xFFFFFFFFFFFFFFFF`|To jump to an address in memory.|
|`JE`|`0x41`|`JE 0xFFFFFFFFFFFFFFFF`|To jump to an address in memory if the comparison register has the equal value.|
|`JNE`|`0x42`|`JNE 0xFFFFFFFFFFFFFFFF`|To jump to an address in memory if the comparison register has an unequal value.|
|`JNZ`|`0x43`|`JNZ 0xFFFFFFFFFFFFFFFF`|To jump to an address in memory if the comparison register has a non-zero value.|
|`CALL`|`0x51`|`CALL 0xFFFFFFFFFFFFFFFF`|To jump to an address in memory.|
|`RET`|`0x52`|`RET`|To return to the previous address.|
|`INC`|`0x61`|`INC R1`|Increase the register by one unit.|
|`DEC`|`0x62`|`DEC R1`|Decrease the register by one unit.|
|`ADD`|`0x66`|`ADD R1, R2`|Adding two registers together.|
|`ADD`|`0x67`|`ADD R1, 0xFF`|Adding a 1-byte number with a register.|
|`ADD`|`0x68`|`ADD R1, 0xFFFF`|Adding a 2-byte number with a register.|
|`ADD`|`0x69`|`ADD R1, 0xFFFFFFFF`|Adding a 4-byte number with a register.|
|`ADD`|`0x70`|`ADD R1, 0xFFFFFFFFFFFFFFFF`|Adding a 8-byte number with a register.|
|`SUB`|`0x76`|`SUB R1, R2`|Minus two registers together.|
|`SUB`|`0x77`|`SUB R1, 0xFF`|Minus a 1-byte number with a register.|
|`SUB`|`0x78`|`SUB R1, 0xFFFF`|Minus a 2-byte number with a register.|
|`SUB`|`0x79`|`SUB R1, 0xFFFFFFFF`|Minus a 4-byte number with a register.|
|`SUB`|`0x80`|`SUB R1, 0xFFFFFFFFFFFFFFFF`|Minus a 8-byte number with a register.|
|`MUL`|`0x86`|`MUL R1, R2`|Multiplication two registers together.|
|`MUL`|`0x87`|`MUL R1, 0xFF`|Multiplication a 1-byte number with a register.|
|`MUL`|`0x88`|`MUL R1, 0xFFFF`|Multiplication a 2-byte number with a register.|
|`MUL`|`0x89`|`MUL R1, 0xFFFFFFFF`|Multiplication a 4-byte number with a register.|
|`MUL`|`0x90`|`MUL R1, 0xFFFFFFFFFFFFFFFF`|Multiplication a 8-byte number with a register.|
|`DIV`|`0x96`|`DIV R1, R2`|Division two registers together.|
|`DIV`|`0x97`|`DIV R1, 0xFF`|Division a 1-byte number with a register.|
|`DIV`|`0x98`|`DIV R1, 0xFFFF`|Division a 2-byte number with a register.|
|`DIV`|`0x99`|`DIV R1, 0xFFFFFFFF`|Division a 4-byte number with a register.|
|`DIV`|`0x9A`|`DIV R1, 0xFFFFFFFFFFFFFFFF`|Division a 8-byte number with a register.|
|`IP`|`0xA0`|`IP`|IP register.|
|`RK`|`0xA1`|`RK`|RK register.|
|`RC`|`0xA2`|`RC`|RC register.|
|`SP`|`0xA3`|`SP`|SP register.|
|`R1`|`0xA4`|`R1`|R1 register.|
|`R2`|`0xA5`|`R2`|R2 register.|
|`R3`|`0xA6`|`R3`|R3 register.|
|`R4`|`0xA7`|`R4`|R4 register.|
|`R5`|`0xA8`|`R5`|R5 register.|
|`R6`|`0xA9`|`R6`|R6 register.|
|`R7`|`0xAA`|`R7`|R7 register.|
|`R8`|`0xAB`|`R8`|R8 register.|
|`RP1`|`0xAC`|`RP1`|RP1 register.|
|`RP2`|`0xAD`|`RP2`|RP2 register.|
|`RP3`|`0xAE`|`RP3`|RP3 register.|
|`RP4`|`0xAF`|`RP4`|RP4 register.|
|`RP5`|`0xB0`|`RP5`|RP5 register.|
|`RP6`|`0xB1`|`RP6`|RP6 register.|
|`RP7`|`0xB2`|`RP7`|RP7 register.|
|`RP8`|`0xB3`|`RP8`|RP8 register.|
|`RR1`|`0xB4`|`RR1`|RR1 register.|
|`RR2`|`0xB5`|`RR2`|RR2 register.|
|`RR3`|`0xB6`|`RR3`|RR3 register.|
|`RR4`|`0xB7`|`RR4`|RR4 register.|
|`RR5`|`0xB8`|`RR5`|RR5 register.|
|`RR6`|`0xB9`|`RR6`|RR6 register.|
|`RR7`|`0xBA`|`RR7`|RR7 register.|
|`RR8`|`0xBB`|`RR8`|RR8 register.|
|`INT`|`0xFF`|`INT`|Interruption.|

## Packets Interface

To communicate with the packets in the X8000 engine, you must place the code related to each function in the RK register, then set its parameters in the RP1...RP8 registers, and then make a system call.

The available system call table is as follows:

|Title|Description|RK|RP1|RP2|RP3|RP4|RP5|RP6|RP7|RP8|RR1|RR2|RR3|RR4|RR5|RR6|RR7|RR8|
|-----|-----------|--|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|write|Writing a buffer|`0x1`|`char file_descriptor`|`char* buffer`|`unsigned long buffer_size`|`void`|`void`|`void`|`void`|`void`|`char status`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|
|read|Read a buffer|`0x2`|`char file_descriptor`|`char* buffer`|`unsigned long buffer_size`|`void`|`void`|`void`|`void`|`void`|`char* buffer`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|
|exit|Exit the program|`0xA`|`int status`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|
|malloc|Memory allocation.|`0x61`|`unsigned long size`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void* address`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|
|realloc|Memory reallocation.|`0x62`|`void* address`|`unsigned long size`|`void`|`void`|`void`|`void`|`void`|`void`|`void* address`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|
|free|Free memory.|`0x63`|`void* address`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|
|wbuff|Write in memory.|`0x64`|`void* address`|`char ch`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|`void`|

The descriptors of the available files are as follows:

|Title|Description|Hex|
|-----|-----------|---|
|STDOUT|Output current|`0x1`|
|STDERR|Output current|`0x2`|
|STDIN|Input current|`0x3`|
