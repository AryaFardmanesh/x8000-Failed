#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// ==================== Program Define ====================
typedef unsigned char ubyte_t;

#define X8000_EXIT_SUCCESS 0x0
#define X8000_EXIT_FAILURE 0x1

ubyte_t* program = NULL;
bool programStatus = true;
long long exitCode = X8000_EXIT_SUCCESS;

void initProgram( ubyte_t* _program );
void freeProgram();
void x8000_exe();
// ==================== Program Define ====================

// ==================== Registers Define ====================
typedef long long register_t;
typedef uintptr_t x8000_address_t;

#define NULL_REG	(register_t) 0x0000000000000000
#define NULL_ADDRESS	(x8000_address_t) 0x0000000000000000
#define NULL_SP		NULL_ADDRESS

#define REGISTER_IP 	(ubyte_t) 0xA0
#define REGISTER_RK 	(ubyte_t) 0xA1
#define REGISTER_RC 	(ubyte_t) 0xA2
#define REGISTER_SP 	(ubyte_t) 0xA3
#define REGISTER_R1 	(ubyte_t) 0xA4
#define REGISTER_R2 	(ubyte_t) 0xA5
#define REGISTER_R3 	(ubyte_t) 0xA6
#define REGISTER_R4 	(ubyte_t) 0xA7
#define REGISTER_R5 	(ubyte_t) 0xA8
#define REGISTER_R6 	(ubyte_t) 0xA9
#define REGISTER_R7 	(ubyte_t) 0xAA
#define REGISTER_R8 	(ubyte_t) 0xAB
#define REGISTER_RP1 	(ubyte_t) 0xAC
#define REGISTER_RP2 	(ubyte_t) 0xAD
#define REGISTER_RP3 	(ubyte_t) 0xAE
#define REGISTER_RP4 	(ubyte_t) 0xAF
#define REGISTER_RP5 	(ubyte_t) 0xB0
#define REGISTER_RP6 	(ubyte_t) 0xB1
#define REGISTER_RP7 	(ubyte_t) 0xB2
#define REGISTER_RP8 	(ubyte_t) 0xB3
#define REGISTER_RR1 	(ubyte_t) 0xB4
#define REGISTER_RR2 	(ubyte_t) 0xB5
#define REGISTER_RR3 	(ubyte_t) 0xB6
#define REGISTER_RR4 	(ubyte_t) 0xB7
#define REGISTER_RR5	(ubyte_t) 0xB8
#define REGISTER_RR6	(ubyte_t) 0xB9
#define REGISTER_RR7	(ubyte_t) 0xBA
#define REGISTER_RR8	(ubyte_t) 0xBB

struct RegistersStruct {
	register_t IP	;
	register_t RK	;
	register_t RC	;
	register_t SP	;
	register_t R1	;
	register_t R2	;
	register_t R3	;
	register_t R4	;
	register_t R5	;
	register_t R6	;
	register_t R7	;
	register_t R8	;
	register_t RP1	;
	register_t RP2	;
	register_t RP3	;
	register_t RP4	;
	register_t RP5	;
	register_t RP6	;
	register_t RP7	;
	register_t RP8	;
	register_t RR1	;
	register_t RR2	;
	register_t RR3	;
	register_t RR4	;
	register_t RR5	;
	register_t RR6	;
	register_t RR7	;
	register_t RR8	;
};

struct RegistersStruct registers;
x8000_address_t* stackPointer = NULL;
size_t stackPointerSize = 0;

void initRegisters();
void freeRegisters();
void resetRegisters();
bool isValidRegister( ubyte_t reg );
void setRegister( ubyte_t reg, register_t val );
register_t getRegister( ubyte_t reg );
void pushSP( x8000_address_t address );
x8000_address_t popSP();
ubyte_t instructionPeek();
ubyte_t instructionNext();
// ==================== Registers Define ====================

// ==================== Instruction Define ====================
#define X8000_MOV_R	(ubyte_t) 0x20
#define X8000_MOV_8	(ubyte_t) 0x21
#define X8000_MOV_16	(ubyte_t) 0x22
#define X8000_MOV_32	(ubyte_t) 0x23
#define X8000_MOV_64	(ubyte_t) 0x24
#define X8000_CMP_R	(ubyte_t) 0x31
#define X8000_CMP_8	(ubyte_t) 0x32
#define X8000_CMP_16	(ubyte_t) 0x33
#define X8000_CMP_32	(ubyte_t) 0x34
#define X8000_CMP_64	(ubyte_t) 0x35
#define X8000_JMP	(ubyte_t) 0x40
#define X8000_JE	(ubyte_t) 0x41
#define X8000_JNE	(ubyte_t) 0x42
#define X8000_JNZ	(ubyte_t) 0x43
#define X8000_CALL	(ubyte_t) 0x51
#define X8000_RET	(ubyte_t) 0x52
#define X8000_INC	(ubyte_t) 0x61
#define X8000_DEC	(ubyte_t) 0x62
#define X8000_ADD_R	(ubyte_t) 0x66
#define X8000_ADD_8	(ubyte_t) 0x67
#define X8000_ADD_16	(ubyte_t) 0x68
#define X8000_ADD_32	(ubyte_t) 0x69
#define X8000_ADD_64	(ubyte_t) 0x70
#define X8000_SUB_R	(ubyte_t) 0x76
#define X8000_SUB_8	(ubyte_t) 0x77
#define X8000_SUB_16	(ubyte_t) 0x78
#define X8000_SUB_32	(ubyte_t) 0x79
#define X8000_SUB_64	(ubyte_t) 0x80
#define X8000_MUL_R	(ubyte_t) 0x86
#define X8000_MUL_8	(ubyte_t) 0x87
#define X8000_MUL_16	(ubyte_t) 0x88
#define X8000_MUL_32	(ubyte_t) 0x89
#define X8000_MUL_64	(ubyte_t) 0x90
#define X8000_DIV_R	(ubyte_t) 0x96
#define X8000_DIV_8	(ubyte_t) 0x97
#define X8000_DIV_16	(ubyte_t) 0x98
#define X8000_DIV_32	(ubyte_t) 0x99
#define X8000_DIV_64	(ubyte_t) 0x9A
#define X8000_INT	(ubyte_t) 0xFF

#define INSTRUCTION_STATUS_SUCCESS (ubyte_t) 0x00
#define INSTRUCTION_STATUS_FAILURE (ubyte_t) 0x01

/*
	Cmp Flag Bits:
		IN 0 1 2 3 4 5 6 7
		BT 0 0 0 0 0 0 0 0
	0: Not Zero
	1: Equal
*/
#define CMP_FLAG_EQ (ubyte_t) 0b01000000
#define CMP_FLAG_NJ (ubyte_t) 0b10000000

ubyte_t handleInstruction( ubyte_t ins );
ubyte_t x8000_mov();
ubyte_t x8000_cmp();
ubyte_t x8000_jmp();
ubyte_t x8000_je();
ubyte_t x8000_jne();
ubyte_t x8000_jnz();
ubyte_t x8000_call();
ubyte_t x8000_ret();
ubyte_t x8000_inc();
ubyte_t x8000_dec();
ubyte_t x8000_add();
ubyte_t x8000_sub();
ubyte_t x8000_mul();
ubyte_t x8000_div();
ubyte_t x8000_int();
// ==================== Instruction Define ====================

// ==================== Syscall Define ====================
#define SYSCALL_STATUS_SUCCESS (ubyte_t) 0x00
#define SYSCALL_STATUS_FAILURE (ubyte_t) 0x01

#define SYSCALL_CODE_WRITE	(ubyte_t) 0x01
#define SYSCALL_CODE_READ	(ubyte_t) 0x02
#define SYSCALL_CODE_EXIT	(ubyte_t) 0x0A
#define SYSCALL_CODE_MALLOC	(ubyte_t) 0x61
#define SYSCALL_CODE_REALLOC	(ubyte_t) 0x62
#define SYSCALL_CODE_FREE	(ubyte_t) 0x63
#define SYSCALL_CODE_WBUFF	(ubyte_t) 0x64

#define FILE_DESCRIPTOR_STDOUT	(ubyte_t) 0x01
#define FILE_DESCRIPTOR_STDERR	(ubyte_t) 0x02
#define FILE_DESCRIPTOR_STDIN	(ubyte_t) 0x03

ubyte_t syscall(
	register_t rk,
	register_t rp1,
	register_t rp2,
	register_t rp3,
	register_t rp4,
	register_t rp5,
	register_t rp6,
	register_t rp7,
	register_t rp8
);
ubyte_t syscall_write( register_t file_descriptor, x8000_address_t buff, size_t buff_size );
ubyte_t syscall_read( register_t file_descriptor, x8000_address_t buff, size_t buff_size );
ubyte_t syscall_exit( register_t status );
x8000_address_t syscall_malloc( size_t buff_size );
x8000_address_t syscall_realloc( x8000_address_t address, size_t new_size );
ubyte_t syscall_free( x8000_address_t address );
ubyte_t syscall_wbuff( x8000_address_t address, char ch );
// ==================== Syscall Define ====================

// ==================== X8000 Define ====================
void x8000_init( ubyte_t* _program );
void x8000_free();
// ==================== X8000 Define ====================

// ==================== Registers ====================
void initRegisters() {
	resetRegisters();

	stackPointer = (x8000_address_t*)malloc( stackPointerSize );
}

void freeRegisters() {
	if ( stackPointer != NULL ) free( stackPointer );
}

void resetRegisters() {
	registers.IP = (register_t)-0x1;
	registers.RK = (register_t)0x0;
	registers.RC = (register_t)0x0;
	registers.SP = (register_t)0x0;
	registers.R1 = (register_t)0x0;
	registers.R2 = (register_t)0x0;
	registers.R3 = (register_t)0x0;
	registers.R4 = (register_t)0x0;
	registers.R5 = (register_t)0x0;
	registers.R6 = (register_t)0x0;
	registers.R7 = (register_t)0x0;
	registers.R8 = (register_t)0x0;
	registers.RP1 = (register_t)0x0;
	registers.RP2 = (register_t)0x0;
	registers.RP3 = (register_t)0x0;
	registers.RP4 = (register_t)0x0;
	registers.RP5 = (register_t)0x0;
	registers.RP6 = (register_t)0x0;
	registers.RP7 = (register_t)0x0;
	registers.RP8 = (register_t)0x0;
	registers.RR1 = (register_t)0x0;
	registers.RR2 = (register_t)0x0;
	registers.RR3 = (register_t)0x0;
	registers.RR4 = (register_t)0x0;
	registers.RR5 = (register_t)0x0;
	registers.RR6 = (register_t)0x0;
	registers.RR7 = (register_t)0x0;
	registers.RR8 = (register_t)0x0;
}

bool isValidRegister( ubyte_t reg ) {
	if ( reg >= (ubyte_t)0xA0 && reg <= (ubyte_t)0xBB ) {
		return true;
	}
	return false;
}

void setRegister( ubyte_t reg, register_t val ) {
	switch ( reg ) {
	case REGISTER_IP:
		registers.IP = val;
		break;
	case REGISTER_RK:
		registers.RK = val;
		break;
	case REGISTER_RC:
		registers.RC = val;
		break;
	case REGISTER_SP:
		registers.SP = val;
		break;
	case REGISTER_R1:
		registers.R1 = val;
		break;
	case REGISTER_R2:
		registers.R2 = val;
		break;
	case REGISTER_R3:
		registers.R3 = val;
		break;
	case REGISTER_R4:
		registers.R4 = val;
		break;
	case REGISTER_R5:
		registers.R5 = val;
		break;
	case REGISTER_R6:
		registers.R6 = val;
		break;
	case REGISTER_R7:
		registers.R7 = val;
		break;
	case REGISTER_R8:
		registers.R8 = val;
		break;
	case REGISTER_RP1:
		registers.RP1 = val;
		break;
	case REGISTER_RP2:
		registers.RP2 = val;
		break;
	case REGISTER_RP3:
		registers.RP3 = val;
		break;
	case REGISTER_RP4:
		registers.RP4 = val;
		break;
	case REGISTER_RP5:
		registers.RP5 = val;
		break;
	case REGISTER_RP6:
		registers.RP6 = val;
		break;
	case REGISTER_RP7:
		registers.RP7 = val;
		break;
	case REGISTER_RP8:
		registers.RP8 = val;
		break;
	case REGISTER_RR1:
		registers.RR1 = val;
		break;
	case REGISTER_RR2:
		registers.RR2 = val;
		break;
	case REGISTER_RR3:
		registers.RR3 = val;
		break;
	case REGISTER_RR4:
		registers.RR4 = val;
		break;
	case REGISTER_RR5:
		registers.RR5 = val;
		break;
	case REGISTER_RR6:
		registers.RR6 = val;
		break;
	case REGISTER_RR7:
		registers.RR7 = val;
		break;
	case REGISTER_RR8:
		registers.RR8 = val;
		break;
	}
}

register_t getRegister( ubyte_t reg ) {
	switch ( reg ) {
	case REGISTER_IP:
		return registers.IP;
	case REGISTER_RK:
		return registers.RK;
	case REGISTER_RC:
		return registers.RC;
	case REGISTER_SP:
		return registers.SP;
	case REGISTER_R1:
		return registers.R1;
	case REGISTER_R2:
		return registers.R2;
	case REGISTER_R3:
		return registers.R3;
	case REGISTER_R4:
		return registers.R4;
	case REGISTER_R5:
		return registers.R5;
	case REGISTER_R6:
		return registers.R6;
	case REGISTER_R7:
		return registers.R7;
	case REGISTER_R8:
		return registers.R8;
	case REGISTER_RP1:
		return registers.RP1;
	case REGISTER_RP2:
		return registers.RP2;
	case REGISTER_RP3:
		return registers.RP3;
	case REGISTER_RP4:
		return registers.RP4;
	case REGISTER_RP5:
		return registers.RP5;
	case REGISTER_RP6:
		return registers.RP6;
	case REGISTER_RP7:
		return registers.RP7;
	case REGISTER_RP8:
		return registers.RP8;
	case REGISTER_RR1:
		return registers.RR1;
	case REGISTER_RR2:
		return registers.RR2;
	case REGISTER_RR3:
		return registers.RR3;
	case REGISTER_RR4:
		return registers.RR4;
	case REGISTER_RR5:
		return registers.RR5;
	case REGISTER_RR6:
		return registers.RR6;
	case REGISTER_RR7:
		return registers.RR7;
	case REGISTER_RR8:
		return registers.RR8;
	default:
		return NULL_REG;
	}
}

void pushSP( x8000_address_t address ) {
	stackPointer = (x8000_address_t*)malloc( ++stackPointerSize );
	stackPointer[ stackPointerSize - 1 ] = address;
}

x8000_address_t popSP() {
	if ( stackPointer == 0 ) {
		return NULL_SP;
	}

	x8000_address_t address = stackPointer[ --stackPointerSize ];
	stackPointer = (x8000_address_t*)malloc( stackPointerSize );

	return address;
}

ubyte_t instructionPeek() {
	return program[ registers.IP ];
}

ubyte_t instructionNext() {
	registers.IP++;
	return instructionPeek();
}
// ==================== Registers ====================

// ==================== Instruction ====================
ubyte_t handleInstruction( ubyte_t ins ) {
	switch ( ins ) {
	case X8000_MOV_R:
	case X8000_MOV_8:
	case X8000_MOV_16:
	case X8000_MOV_32:
	case X8000_MOV_64:
		return x8000_mov();
	case X8000_CMP_R:
	case X8000_CMP_8:
	case X8000_CMP_16:
	case X8000_CMP_32:
	case X8000_CMP_64:
		return x8000_cmp();
	case X8000_JMP:
		return x8000_jmp();
	case X8000_JE:
		return x8000_je();
	case X8000_JNE:
		return x8000_jne();
	case X8000_JNZ:
		return x8000_jnz();
	case X8000_CALL:
		return x8000_call();
	case X8000_RET:
		return x8000_ret();
	case X8000_INC:
		return x8000_inc();
	case X8000_DEC:
		return x8000_dec();
	case X8000_ADD_R:
	case X8000_ADD_8:
	case X8000_ADD_16:
	case X8000_ADD_32:
	case X8000_ADD_64:
		return x8000_add();
	case X8000_SUB_R:
	case X8000_SUB_8:
	case X8000_SUB_16:
	case X8000_SUB_32:
	case X8000_SUB_64:
		return x8000_sub();
	case X8000_MUL_R:
	case X8000_MUL_8:
	case X8000_MUL_16:
	case X8000_MUL_32:
	case X8000_MUL_64:
		return x8000_mul();
	case X8000_DIV_R:
	case X8000_DIV_8:
	case X8000_DIV_16:
	case X8000_DIV_32:
	case X8000_DIV_64:
		return x8000_div();
	case X8000_INT:
		return x8000_int();
	default:
		return INSTRUCTION_STATUS_FAILURE;
	}
}

ubyte_t x8000_mov() {
	// MOV RK, 0xFF

	ubyte_t mode = instructionPeek();
	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	if ( mode == X8000_MOV_R ) {
		ubyte_t vreg = instructionNext();
		if ( isValidRegister( vreg ) == false ) {
			return INSTRUCTION_STATUS_FAILURE;
		}
		setRegister( reg, getRegister( vreg ) );
		return INSTRUCTION_STATUS_SUCCESS;
	}

	union buffUnion {
		ubyte_t bt[ 8 ];
		register_t reg;
	};

	union buffUnion buffVal;
	buffVal.reg = 0;

	if ( mode == X8000_MOV_8 ) {
		buffVal.bt[ 0 ] = instructionNext();
		setRegister( reg, buffVal.reg );
	}else if ( mode == X8000_MOV_16 ) {
		buffVal.bt[ 0 ] = instructionNext();
		buffVal.bt[ 1 ] = instructionNext();
		setRegister( reg, buffVal.reg );
	}else if ( mode == X8000_MOV_32 ) {
		buffVal.bt[ 0 ] = instructionNext();
		buffVal.bt[ 1 ] = instructionNext();
		buffVal.bt[ 2 ] = instructionNext();
		buffVal.bt[ 3 ] = instructionNext();
		setRegister( reg, buffVal.reg );
	}else if ( mode == X8000_MOV_64 ) {
		buffVal.bt[ 0 ] = instructionNext();
		buffVal.bt[ 1 ] = instructionNext();
		buffVal.bt[ 2 ] = instructionNext();
		buffVal.bt[ 3 ] = instructionNext();
		buffVal.bt[ 4 ] = instructionNext();
		buffVal.bt[ 5 ] = instructionNext();
		buffVal.bt[ 6 ] = instructionNext();
		buffVal.bt[ 7 ] = instructionNext();
		setRegister( reg, buffVal.reg );
	}else {
		return INSTRUCTION_STATUS_FAILURE;
	}

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_cmp() {
	// CMP RK, 0xFF

	ubyte_t mode = instructionPeek();
	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	registers.RC = (register_t)0x0;
	register_t r1 = getRegister( reg );
	register_t r2;

	// Used for non-register value
	union {
		ubyte_t bt[ 8 ];
		x8000_address_t value;
	} buffUnion;
	buffUnion.value = 0;

	if ( mode == X8000_CMP_R ) {
		ubyte_t vreg = instructionNext();

		if ( isValidRegister( vreg ) == false ) {
			return INSTRUCTION_STATUS_FAILURE;
		}

		r2 = getRegister( vreg );
	}else if ( mode == X8000_CMP_8 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		r2 = buffUnion.value;
	}else if ( mode == X8000_CMP_16 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		r2 = buffUnion.value;
	}else if ( mode == X8000_CMP_32 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		r2 = buffUnion.value;
	}else if ( mode == X8000_CMP_64 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		buffUnion.bt[ 4 ] = instructionNext();
		buffUnion.bt[ 5 ] = instructionNext();
		buffUnion.bt[ 6 ] = instructionNext();
		buffUnion.bt[ 7 ] = instructionNext();
		r2 = buffUnion.value;
	}

	if ( r1 == r2 ) registers.RC |= CMP_FLAG_EQ;
	if ( r1 != 0 ) registers.RC |= CMP_FLAG_NJ;

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_jmp() {
	// JMP 0xFFFFFFFFFFFFFFFF

	union {
		ubyte_t bt[ 8 ];
		x8000_address_t value;
	} buff;
	buff.value = 0;

	for ( int i = 0; i < 8; i++ ) {
		buff.bt[ i ] = instructionNext();
	}

	setRegister( REGISTER_IP, buff.value );

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_je() {
	union {
		ubyte_t bt[ 8 ];
		x8000_address_t value;
	} buffAddress;
	buffAddress.value = 0;

	for ( int i = 0; i < 8; i++ ) {
		buffAddress.bt[ i ] = instructionNext();
	}

	register_t flag = registers.RC;

	flag <<= 1;
	flag >>= 7;

	if ( flag ) {
		registers.IP = buffAddress.value;
	}

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_jne() {
	union {
		ubyte_t bt[ 8 ];
		x8000_address_t value;
	} buffAddress;
	buffAddress.value = 0;

	for ( int i = 0; i < 8; i++ ) {
		buffAddress.bt[ i ] = instructionNext();
	}

	register_t flag = registers.RC;

	flag <<= 1;
	flag >>= 7;

	if ( !flag ) {
		registers.IP = buffAddress.value;
	}

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_jnz() {
	union {
		ubyte_t bt[ 8 ];
		x8000_address_t value;
	} buffAddress;
	buffAddress.value = 0;

	for ( int i = 0; i < 8; i++ ) {
		buffAddress.bt[ i ] = instructionNext();
	}

	register_t flag = registers.RC;

	flag >>= 7;

	if ( flag ) {
		registers.IP = buffAddress.value;
	}

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_call() {
	// CALL 0xFFFFFFFFFFFFFFFF

	union {
		ubyte_t bt[ 8 ];
		x8000_address_t value;
	} buffAddress;
	buffAddress.value = 0;

	for ( int i = 0; i < 8; i++ ) {
		buffAddress.bt[ i ] = instructionNext();
	}

	pushSP( registers.IP );
	registers.IP = buffAddress.value;

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_ret() {
	x8000_address_t address = popSP();

	if ( address == NULL_ADDRESS ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	registers.IP = address;

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_inc() {
	// INC RK

	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	setRegister( reg, getRegister( reg ) + 1 );

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_dec() {
		// INC RK

	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	setRegister( reg, getRegister( reg ) - 1 );

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_add() {
	// ADD RK, 0xFF

	ubyte_t mode = instructionPeek();
	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	register_t r1 = getRegister( reg );
	register_t r2;

	if ( mode == X8000_ADD_R ) {
		ubyte_t vreg = instructionNext();

		if ( isValidRegister( vreg ) == false ) {
			return INSTRUCTION_STATUS_FAILURE;
		}

		r2 = getRegister( vreg );
		setRegister( reg, ( r1 + r2 ) );

		return INSTRUCTION_STATUS_SUCCESS;
	}

	union {
		ubyte_t bt[ 8 ];
		register_t reg;
	} buffUnion;
	buffUnion.reg = 0;


	if ( mode == X8000_ADD_8 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_ADD_16 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_ADD_32 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_ADD_64 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		buffUnion.bt[ 4 ] = instructionNext();
		buffUnion.bt[ 5 ] = instructionNext();
		buffUnion.bt[ 6 ] = instructionNext();
		buffUnion.bt[ 7 ] = instructionNext();
		r2 = buffUnion.reg;
	}else {
		return INSTRUCTION_STATUS_FAILURE;
	}

	setRegister( reg, ( r1 + r2 ) );

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_sub() {
	// SUB RK, 0xFF

	ubyte_t mode = instructionPeek();
	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	register_t r1 = getRegister( reg );
	register_t r2;

	if ( mode == X8000_SUB_R ) {
		ubyte_t vreg = instructionNext();

		if ( isValidRegister( vreg ) == false ) {
			return INSTRUCTION_STATUS_FAILURE;
		}

		r2 = getRegister( vreg );
		setRegister( reg, ( r1 - r2 ) );

		return INSTRUCTION_STATUS_SUCCESS;
	}

	union {
		ubyte_t bt[ 8 ];
		register_t reg;
	} buffUnion;
	buffUnion.reg = 0;

	if ( mode == X8000_SUB_8 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_SUB_16 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_SUB_32 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_SUB_64 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		buffUnion.bt[ 4 ] = instructionNext();
		buffUnion.bt[ 5 ] = instructionNext();
		buffUnion.bt[ 6 ] = instructionNext();
		buffUnion.bt[ 7 ] = instructionNext();
		r2 = buffUnion.reg;
	}else {
		return INSTRUCTION_STATUS_FAILURE;
	}

	setRegister( reg, ( r1 - r2 ) );

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_mul() {
	// MUL RK, 0xFF

	ubyte_t mode = instructionPeek();
	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	register_t r1 = getRegister( reg );
	register_t r2;

	if ( mode == X8000_MUL_R ) {
		ubyte_t vreg = instructionNext();

		if ( isValidRegister( vreg ) == false ) {
			return INSTRUCTION_STATUS_FAILURE;
		}

		r2 = getRegister( vreg );
		setRegister( reg, ( r1 * r2 ) );

		return INSTRUCTION_STATUS_SUCCESS;
	}

	union {
		ubyte_t bt[ 8 ];
		register_t reg;
	} buffUnion;
	buffUnion.reg = 0;


	if ( mode == X8000_MUL_8 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_MUL_16 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_MUL_32 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_MUL_64 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		buffUnion.bt[ 4 ] = instructionNext();
		buffUnion.bt[ 5 ] = instructionNext();
		buffUnion.bt[ 6 ] = instructionNext();
		buffUnion.bt[ 7 ] = instructionNext();
		r2 = buffUnion.reg;
	}else {
		return INSTRUCTION_STATUS_FAILURE;
	}

	setRegister( reg, ( r1 * r2 ) );

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_div() {
	// DIV RK, 0xFF

	ubyte_t mode = instructionPeek();
	ubyte_t reg = instructionNext();

	if ( isValidRegister( reg ) == false ) {
		return INSTRUCTION_STATUS_FAILURE;
	}

	register_t r1 = getRegister( reg );
	register_t r2;

	if ( mode == X8000_DIV_R ) {
		ubyte_t vreg = instructionNext();

		if ( isValidRegister( vreg ) == false ) {
			return INSTRUCTION_STATUS_FAILURE;
		}

		r2 = getRegister( vreg );

		if ( r1 == 0 || r2 == 0 ) {
			setRegister( reg, 0 );
		}else {
			setRegister( reg, ( r1 / r2 ) );
		}

		return INSTRUCTION_STATUS_SUCCESS;
	}

	union {
		ubyte_t bt[ 8 ];
		register_t reg;
	} buffUnion;
	buffUnion.reg = 0;


	if ( mode == X8000_DIV_8 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_DIV_16 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_DIV_32 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		r2 = buffUnion.reg;
	}else if ( mode == X8000_DIV_64 ) {
		buffUnion.bt[ 0 ] = instructionNext();
		buffUnion.bt[ 1 ] = instructionNext();
		buffUnion.bt[ 2 ] = instructionNext();
		buffUnion.bt[ 3 ] = instructionNext();
		buffUnion.bt[ 4 ] = instructionNext();
		buffUnion.bt[ 5 ] = instructionNext();
		buffUnion.bt[ 6 ] = instructionNext();
		buffUnion.bt[ 7 ] = instructionNext();
		r2 = buffUnion.reg;
	}else {
		return INSTRUCTION_STATUS_FAILURE;
	}

	if ( r1 == 0 || r2 == 0 ) {
		setRegister( reg, 0 );
	}else {
		setRegister( reg, ( r1 / r2 ) );
	}

	return INSTRUCTION_STATUS_SUCCESS;
}

ubyte_t x8000_int() {
	return syscall(
		registers.RK,
		registers.RP1,
		registers.RP2,
		registers.RP3,
		registers.RP4,
		registers.RP5,
		registers.RP6,
		registers.RP7,
		registers.RP8
	);
}
// ==================== Instruction ====================

// ==================== Syscall ====================
ubyte_t syscall(
	register_t rk,
	register_t rp1,
	register_t rp2,
	register_t rp3,
	register_t rp4,
	register_t rp5,
	register_t rp6,
	register_t rp7,
	register_t rp8
) {
	switch ( rk ) {
	case SYSCALL_CODE_WRITE: {
		return syscall_write( rp1, (x8000_address_t)rp2, (size_t)rp3 );
	}
	case SYSCALL_CODE_READ: {
		return syscall_read( rp1, (x8000_address_t)rp2, (size_t)rp3 );
	}
	case SYSCALL_CODE_EXIT: {
		return syscall_exit( rp1 );
	}
	case SYSCALL_CODE_MALLOC: {
		x8000_address_t address = syscall_malloc( rp1 );
		if ( address == (x8000_address_t)NULL ) {
			return SYSCALL_STATUS_FAILURE;
		}
		registers.RR1 = (x8000_address_t)address;
		return SYSCALL_STATUS_SUCCESS;
	}
	case SYSCALL_CODE_REALLOC: {
		x8000_address_t address = syscall_realloc( (x8000_address_t)rp1, rp2 );
		if ( address == (x8000_address_t)NULL ) {
			return SYSCALL_STATUS_FAILURE;
		}
		registers.RR1 = (x8000_address_t)address;
		return SYSCALL_STATUS_SUCCESS;
	}
	case SYSCALL_CODE_FREE: {
		return syscall_free( (x8000_address_t)rp1 );
	}
	case SYSCALL_CODE_WBUFF: {
		return syscall_wbuff( (x8000_address_t)rp1, (char)rp2 );
	}
	default:
		return SYSCALL_STATUS_FAILURE;
	}
}

ubyte_t syscall_write( register_t file_descriptor, x8000_address_t buff, size_t buff_size ) {
	if ( file_descriptor == FILE_DESCRIPTOR_STDOUT ) {
		write( STDOUT_FILENO, (void*)buff, buff_size );
	}else if ( file_descriptor == FILE_DESCRIPTOR_STDERR ) {
		write( STDERR_FILENO, (void*)buff, buff_size );
	}else {
		return SYSCALL_STATUS_FAILURE;
	}

	return SYSCALL_STATUS_SUCCESS;
}

ubyte_t syscall_read( register_t file_descriptor, x8000_address_t buff, size_t buff_size ) {
	if ( file_descriptor == FILE_DESCRIPTOR_STDIN ) {
		ssize_t res = read( STDIN_FILENO, (void*)buff, buff_size );
		return res > 0 ? SYSCALL_STATUS_SUCCESS : SYSCALL_STATUS_FAILURE;
	}else {
		return SYSCALL_STATUS_FAILURE;
	}
}

ubyte_t syscall_exit( register_t status ) {
	programStatus = false;
	exitCode = status;
	return SYSCALL_STATUS_SUCCESS;
}

x8000_address_t syscall_malloc( size_t buff_size ) {
	return (x8000_address_t)malloc( buff_size );
}

x8000_address_t syscall_realloc( x8000_address_t address, size_t new_size ) {
	return (x8000_address_t)realloc( (void*)address, new_size );
}

ubyte_t syscall_free( x8000_address_t address ) {
	free( (void*)address );
	return SYSCALL_STATUS_SUCCESS;
}

ubyte_t syscall_wbuff( x8000_address_t address, char ch ) {
	if ( address == NULL_ADDRESS ) {
		return SYSCALL_STATUS_FAILURE;
	}

	char* ptr = (char*)address;
	*ptr = ch;

	return SYSCALL_STATUS_SUCCESS;
}
// ==================== Syscall ====================

// ==================== Program ====================
void initProgram( ubyte_t* _program ) {
	program = _program;
}

void freeProgram() {
	free( program );
}

void x8000_exe() {
	while ( programStatus ) {
		ubyte_t ins = instructionNext();
		ubyte_t res = handleInstruction( ins );

		if ( res == INSTRUCTION_STATUS_FAILURE ) {
			programStatus = false;

			if ( exitCode == X8000_EXIT_SUCCESS ) {
				exitCode = X8000_EXIT_FAILURE;
			}
		}
	}
}
// ==================== Program ====================

// ==================== X8000 ====================
void x8000_init( ubyte_t* _program ) {
	initProgram( _program );
	initRegisters();
}

void x8000_free() {
	freeProgram();
	freeRegisters();
}
// ==================== X8000 ====================

// ==================== Main ====================
int main( int argc, char* argv[] ) {
	if ( argc != 2 ) {
		if ( argc == 1 ) {
			fprintf( stdout, "Error: No file specified.\n" );
			exit( EXIT_FAILURE );
		}else {
			fprintf( stdout, "Error: Invalid argv.\n" );
			exit( EXIT_FAILURE );
		}
	}

	char* fileAddress = argv[ 1 ];

	FILE* fptr = fopen( fileAddress, "rb" );
	if ( fptr == NULL ) {
		fprintf( stdout, "Error: Cannot open the specified file.\n" );
		exit( EXIT_FAILURE );
	}

	fseek( fptr, 0L, SEEK_END );
	size_t fileSize = ftell( fptr );
	rewind( fptr );

	ubyte_t* file = (ubyte_t*)malloc( fileSize );
	fread( file, 1, fileSize, fptr );

	// printf( "SIZE=%d\n", fileSize );
	// for ( size_t i = 0; i < fileSize; i++ ) {
	// 	printf( "%d\n", file[ i ] );
	// }

	x8000_init( file );
	x8000_exe();

	out:
		x8000_free();
		exit( exitCode );
}
// ==================== Main ====================

// ==================== Example Programs ====================
/*
# Program 1:

Write a program in x8000 to get one char from input and print it.

```c
ubyte_t bin[] = {
	X8000_MOV_8, REGISTER_RK, SYSCALL_CODE_MALLOC,
	X8000_MOV_8, REGISTER_RP1, 0x1,
	X8000_INT,
	X8000_MOV_R, REGISTER_R1, REGISTER_RR1,

	X8000_MOV_8, REGISTER_RK, SYSCALL_CODE_READ,
	X8000_MOV_8, REGISTER_RP1, FILE_DESCRIPTOR_STDIN,
	X8000_MOV_R, REGISTER_RP2, REGISTER_R1,
	X8000_MOV_8, REGISTER_RP3, 0x1,
	X8000_INT,
	X8000_MOV_R, REGISTER_R2, REGISTER_RR1,

	X8000_MOV_8, REGISTER_RK, SYSCALL_CODE_WRITE,
	X8000_MOV_8, REGISTER_RP1, FILE_DESCRIPTOR_STDOUT,
	X8000_MOV_R, REGISTER_RP2, REGISTER_R2,
	X8000_MOV_8, REGISTER_RP3, 0x1,
	X8000_INT,

	X8000_MOV_8, REGISTER_RK, SYSCALL_CODE_FREE,
	X8000_MOV_R, REGISTER_RP1, REGISTER_R1,
	X8000_INT,

	X8000_MOV_8, REGISTER_RK, SYSCALL_CODE_EXIT,
	X8000_MOV_8, REGISTER_RP1, 0x00,
	X8000_INT,
};
```
*/
// ==================== Example Programs ====================
