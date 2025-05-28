#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ==================== TASM Define ====================
char* program = NULL;
size_t programSize = 0;

typedef unsigned char ubyte_t;
typedef ubyte_t token_kind_t;

#define TASM_TOKEN_KIND_EOF		(token_kind_t) 0x00
#define TASM_TOKEN_KIND_KEYWORD		(token_kind_t) 0x01
#define TASM_TOKEN_KIND_ID		(token_kind_t) 0x02
#define TASM_TOKEN_KIND_NUMBER		(token_kind_t) 0x03
#define TASM_TOKEN_KIND_COMMA		(token_kind_t) 0x04
#define TASM_TOKEN_KIND_COLON		(token_kind_t) 0x05

// ==================== X8000 Utils ====================
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

#define SYSCALL_CODE_WRITE	(ubyte_t) 0x01
#define SYSCALL_CODE_READ	(ubyte_t) 0x02
#define SYSCALL_CODE_EXIT	(ubyte_t) 0x0A
#define SYSCALL_CODE_MALLOC	(ubyte_t) 0x61
#define SYSCALL_CODE_REALLOC	(ubyte_t) 0x62
#define SYSCALL_CODE_FREE	(ubyte_t) 0x63

#define FILE_DESCRIPTOR_STDOUT	(ubyte_t) 0x01
#define FILE_DESCRIPTOR_STDERR	(ubyte_t) 0x02
#define FILE_DESCRIPTOR_STDIN	(ubyte_t) 0x03
// ==================== X8000 Utils ====================

// ==================== TASM Keywords ====================
#define TASM_KEYWORD_IP 	"IP"
#define TASM_KEYWORD_RK 	"RK"
#define TASM_KEYWORD_RC 	"RC"
#define TASM_KEYWORD_SP 	"SP"
#define TASM_KEYWORD_R1 	"R1"
#define TASM_KEYWORD_R2 	"R2"
#define TASM_KEYWORD_R3 	"R3"
#define TASM_KEYWORD_R4 	"R4"
#define TASM_KEYWORD_R5 	"R5"
#define TASM_KEYWORD_R6 	"R6"
#define TASM_KEYWORD_R7 	"R7"
#define TASM_KEYWORD_R8 	"R8"
#define TASM_KEYWORD_RP1 	"RP1"
#define TASM_KEYWORD_RP2 	"RP2"
#define TASM_KEYWORD_RP3 	"RP3"
#define TASM_KEYWORD_RP4 	"RP4"
#define TASM_KEYWORD_RP5 	"RP5"
#define TASM_KEYWORD_RP6 	"RP6"
#define TASM_KEYWORD_RP7 	"RP7"
#define TASM_KEYWORD_RP8 	"RP8"
#define TASM_KEYWORD_RR1 	"RR1"
#define TASM_KEYWORD_RR2 	"RR2"
#define TASM_KEYWORD_RR3 	"RR3"
#define TASM_KEYWORD_RR4 	"RR4"
#define TASM_KEYWORD_RR5	"RR5"
#define TASM_KEYWORD_RR6	"RR6"
#define TASM_KEYWORD_RR7	"RR7"
#define TASM_KEYWORD_RR8	"RR8"
#define TASM_KEYWORD_MOV	"MOV"
#define TASM_KEYWORD_CMP	"CMP"
#define TASM_KEYWORD_JMP	"JMP"
#define TASM_KEYWORD_JE		"JE"
#define TASM_KEYWORD_JNE	"JNE"
#define TASM_KEYWORD_JNZ	"JNZ"
#define TASM_KEYWORD_CALL	"CALL"
#define TASM_KEYWORD_RET	"RET"
#define TASM_KEYWORD_INC	"INC"
#define TASM_KEYWORD_DEC	"DEC"
#define TASM_KEYWORD_ADD	"ADD"
#define TASM_KEYWORD_SUB	"SUB"
#define TASM_KEYWORD_MUL	"MUL"
#define TASM_KEYWORD_DIV	"DIV"
#define TASM_KEYWORD_INT	"INT"

#define TASM_MODE_R		(ubyte_t) 0x00
#define TASM_MODE_8		(ubyte_t) 0x08
#define TASM_MODE_16		(ubyte_t) 0x10
#define TASM_MODE_32		(ubyte_t) 0x20
#define TASM_MODE_64		(ubyte_t) 0x40
// ==================== TASM Keywords ====================

#define TASM_KEYWORDS_COUNT 42
char* keywords[] = {
	"IP",
	"RK",
	"RC",
	"SP",
	"R1",
	"R2",
	"R3",
	"R4",
	"R5",
	"R6",
	"R7",
	"R8",
	"RP1",
	"RP2",
	"RP3",
	"RP4",
	"RP5",
	"RP6",
	"RP7",
	"RP8",
	"RR1",
	"RR2",
	"RR3",
	"RR4",
	"RR5",
	"RR6",
	"RR7",
	"RR8",
	"MOV",
	"CMP",
	"JMP",
	"JE",
	"JNZ",
	"CALL",
	"RET",
	"INC",
	"DEC",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"INT",
};

struct token {
	char* value;
	token_kind_t kind;
	struct token* next;
};

struct label {
	char* name;
	size_t pos;
	struct label* next;
};

struct ast {
	struct token* mid;
	struct token* right;
	struct token* left;
	struct ast* next;
};

struct token* tokenStream = NULL;
struct token* tokenStreamHead = NULL;

struct label* labelStream = NULL;
struct label* labelStreamHead = NULL;

struct ast* astStream = NULL;
struct ast* astStreamHead = NULL;

struct token* createToken( char* value, token_kind_t kind );
void appendToken( struct token* _token );
void freeTokens();

struct label* createLabel( char* name, size_t pos );
struct label* searchLabel( char* name );
void appendLabel( struct label* _label );
void freeLabel();

struct ast* createAst( struct token* mid, struct token* right, struct token* left );
void appendAst( struct ast* _ast );
void freeAst();

bool isSeparator( size_t pos );
bool isKeyword( size_t pos );
bool isDigit( size_t pos );
bool isNumber( size_t pos );
char* getKeywordAndId( size_t pos );
char* getNumber( size_t pos );
void tasmLexer();
void printTokenStream();

void tasmParser();
void printAstStream();

ubyte_t* programBin = NULL;
size_t programBinCursor = 0;

#define CONVERT_TOKEN_BYTE_MODE_DEFAULT	(ubyte_t) 0x00
#define CONVERT_TOKEN_BYTE_MODE_REG 	(ubyte_t) 0x00
#define CONVERT_TOKEN_BYTE_MODE_8	(ubyte_t) 0x08
#define CONVERT_TOKEN_BYTE_MODE_16	(ubyte_t) 0x10
#define CONVERT_TOKEN_BYTE_MODE_32	(ubyte_t) 0x20
#define CONVERT_TOKEN_BYTE_MODE_64	(ubyte_t) 0x40

ubyte_t convertTokenToByte( char* symbol, int mode );
ssize_t convertNumberToBytes( char* numstr );
void tasmCodeGen();
void tasmCodeGenFree();

void tasm_init( char* _program, size_t _programSize );
void tasm_free();
// ==================== TASM Define ====================

// ==================== TASM ====================
struct token* createToken( char* value, token_kind_t kind ) {
	struct token* tk = (struct token*)malloc( sizeof( struct token ) );

	tk->value = value;
	tk->kind = kind;
	tk->next = NULL;

	return tk;
}

void appendToken( struct token* _token ) {
	if ( tokenStream == NULL ) {
		tokenStream = _token;
		tokenStreamHead = _token;
		return;
	}

	tokenStreamHead->next = _token;
	tokenStreamHead = tokenStreamHead->next;
}

void freeTokens() {
	struct token* current = tokenStream;

	while ( current != NULL ) {
		struct token* _next = current->next;
		free( current );
		current = _next;
	}
}

struct label* createLabel( char* name, size_t pos ) {
	struct label* lb = (struct label*)malloc( sizeof( struct label ) );

	lb->name = name;
	lb->pos = pos;
	lb->next = NULL;

	return lb;
}

struct label* searchLabel( char* name ) {
	struct label* current = labelStream;

	while ( current != NULL ) {
		if ( strcmp( current->name, name ) == 0 ) {
			return current;
		}

		current = current->next;
	}

	return NULL;
}

void appendLabel( struct label* _label ) {
	if ( labelStream == NULL ) {
		labelStream = _label;
		labelStreamHead = _label;
		return;
	}

	labelStreamHead->next = _label;
	labelStreamHead = labelStreamHead->next;
}

void freeLabel() {
	struct label* current = labelStream;

	while ( current != NULL ) {
		struct label* _next = current->next;
		free( current );
		current = _next;
	}
}

struct ast* createAst( struct token* mid, struct token* right, struct token* left ) {
	struct ast* node = (struct ast*)malloc( sizeof( struct ast ) );

	node->mid = mid;
	node->right = right;
	node->left = left;
	node->next = NULL;

	return node;
}

void appendAst( struct ast* _ast ) {
	if ( astStream == NULL ) {
		astStream = _ast;
		astStreamHead = _ast;
		return;
	}

	astStreamHead->next = _ast;
	astStreamHead = astStreamHead->next;
}

void freeAst() {
	struct ast* current = astStream;

	while ( current != NULL ) {
		struct ast* _next = current->next;
		free( current );
		current = _next;
	}
}

bool isSeparator( size_t pos ) {
	char ch = program[ pos ];

	if ( ch >= 48 && ch <= 57 )
		return false;
	if ( ch >= 65 && ch <= 90 )
		return false;
	if ( ch >= 97 && ch <= 122 )
		return false;
	return true;
}

bool isKeyword( size_t pos ) {
	for ( int ki = 0; ki < TASM_KEYWORDS_COUNT; ki++ ) {
		char* keyword = keywords[ ki ];
		int keyIndex = 0;
		bool isEqual = true;

		for ( size_t i = pos; i < programSize; i++ ) {
			char ch = program[ i ];

			// Convert to uppercase
			if ( ch >= 97 && ch <= 122 ) {
				ch -= 32;
			}

			if ( ch != keyword[ keyIndex++ ] ) {
				isEqual = false;
				break;
			}
			if ( keyword[ keyIndex ] == (char)0x00 ) {
				break;
			}
		}

		if ( isEqual ) {
			return true;
		}
	}

	return false;
}

bool isDigit( size_t pos ) {
	char ch = program[ pos ];

	if ( ch >= '0' && ch <= '9' ) {
		return true;
	}

	return false;
}

bool isNumber( size_t pos ) {
	if ( isDigit( pos ) ) {
		return true;
	}

	char ch = program[ pos ];

	if ( ch == '+' || ch == '-' ) {
		ch = program[ pos + 1 ];

		if ( isDigit( ch ) ) {
			return true;
		}
	}

	return false;
}

char* getKeywordAndId( size_t pos ) {
	int size = 0;
	char* track = (char*)malloc( size );

	for ( size_t i = pos; i < programSize; i++ ) {
		char ch = program[ i ];

		// Need fix -> isSeparator fn
		if ( isSeparator( i ) ) {
			break;
		}

		track = (char*)realloc( track, ++size );
		track[ size - 1 ] = ch;
	}

	track = (char*)realloc( track, ++size );
	track[ size - 1 ] = (char)0x00;

	return track;
}

char* getNumber( size_t pos ) {
	int size = 1;
	char* track = (char*)malloc( size );

	track[ 0 ] = program[ pos++ ];

	char ch = program[ pos ];

	if ( ch == 'x' || ch == 'X' ) {
		// Hex
		track = (char*)realloc( track, ++size );
		track[ size - 1 ] = 'x';

		for ( size_t i = pos + 1; i < programSize; i++ ) {
			char ch = program[ i ];

			if ( !isDigit( i ) && !( ( ch >= 'A' && ch <= 'F' ) || ( ch >= 'a' && ch <= 'f' ) ) ) {
					break;
			}

			track = (char*)realloc( track, ++size );
			track[ size - 1 ] = ch;
		}

		track = (char*)realloc( track, ++size );
		track[ size - 1 ] = (char)0x00;

		return track;
	}

	// Decimal
	for ( size_t i = pos; i < programSize; i++ ) {
		ch = program[ i ];

		track = (char*)realloc( track, ++size );
		track[ size - 1 ] = ch;

		if ( !isDigit( ch ) ) {
			break;
		}
	}

	track = (char*)realloc( track, ++size );
	track[ size - 1 ] = (char)0x00;

	return track;
}

void tasmLexer() {
	size_t i = 0;

	while ( i < programSize ) {
		char ch  = program[ i++ ];

		if ( ch == (char)0x00 ) {
			appendToken( createToken( (char)0x00, TASM_TOKEN_KIND_EOF ) );
			break;
		}else if ( ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t' ) {
			continue;
		}else if ( ch == ';' ) {
			while ( i < programSize && ( ch == '\n' || ch == (char)0x00 ) ) {
				ch = program[ i++ ];
			}
			continue;
		}else if ( ch == ',' ) {
			appendToken( createToken( ",", TASM_TOKEN_KIND_COMMA ) );
			continue;
		}else if ( ch == ':' ) {
			appendToken( createToken( ":", TASM_TOKEN_KIND_COLON ) );
			continue;
		}else if ( isNumber( i - 1 ) ) {
			char* numstr = getNumber( i - 1 );
			i += strlen( numstr );
			appendToken( createToken( numstr, TASM_TOKEN_KIND_NUMBER ) );
			continue;
		}else if ( isKeyword( i - 1 ) ) {
			char* keywordstr = getKeywordAndId( i - 1 );
			size_t keywordstrlen = strlen( keywordstr );
			for ( size_t i = 0; i < keywordstrlen; i++ ) {
				// Convert to uppercase
				if ( keywordstr[ i ] >= 97 && keywordstr[ i ] <= 122 ) {
					keywordstr[ i ] -= 32;
				}
			}
			i += ( keywordstrlen - 1 );
			appendToken( createToken( keywordstr, TASM_TOKEN_KIND_KEYWORD ) );
			continue;
		}else {
			char* idstr = getKeywordAndId( i - 1 );
			i += strlen( idstr ) - 1;
			appendToken( createToken( idstr, TASM_TOKEN_KIND_ID ) );
			continue;
		}
	}
}

void printTokenStream() {
	struct token* current = tokenStream;

	while ( current != NULL ) {
		printf(
			"TOKEN:\n"
			"  TOKEN_STREAM->VALUE='%s'\n"
			"  TOKEN_STREAM->KIND='%d'\n",
			current->value,	current->kind
		);
		current = current->next;
	}
}

void tasmParser() {
	struct token* current = tokenStream;

	while ( current != NULL ) {
		if ( current->kind == TASM_TOKEN_KIND_ID ) {
			if ( current->next != NULL ) {
				if ( current->next->kind == TASM_TOKEN_KIND_COLON ) {
					appendAst(
						createAst( current, NULL, NULL )
					);
					current = current->next;
					continue;
				}
			}
		}else if ( current->kind == TASM_TOKEN_KIND_KEYWORD ) {
			if (
				strcmp( current->value, TASM_KEYWORD_MOV ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_CMP ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_ADD ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_SUB ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_MUL ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_DIV ) == 0
			) {
				/*
					TOKEN:
						MOV RK, 0xFF
					AST:
					        MOV
						/\
					     L /  \ R
					    0xFF  RK
				*/

				struct ast* node = createAst( current, NULL, NULL );

				current = current->next;
				if ( current == NULL ) {
					appendAst( node );
					continue;
				}
				node->right = current;

				current = current->next;
				if ( current == NULL ) {
					continue;
				}

				current = current->next;
				if ( current == NULL ) {
					appendAst( node );
					continue;
				}
				node->left = current;

				appendAst( node );
			}else if (
				strcmp( current->value, TASM_KEYWORD_JMP ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_JE ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_JNZ ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_CALL ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_INC ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_DEC ) == 0
			) {
				/*
					TOKEN:
						JMP __LABEL__
					AST:
					        JMP
						/\
					     L /  \ R
					  (NULL) __LABEL__
				*/

				struct ast* node = createAst( current, NULL, NULL );

				current = current->next;
				if ( current == NULL ) {
					appendAst( node );
					continue;
				}
				node->right = current;

				appendAst( node );
			}else if (
				strcmp( current->value, TASM_KEYWORD_INT ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RET ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_IP ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RK ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RC ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_SP ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R1 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R2 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R3 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R4 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R5 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R6 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R7 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_R8 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP1 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP2 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP3 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP4 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP5 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP6 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP7 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RP8 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR1 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR2 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR3 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR4 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR5 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR6 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR7 ) == 0 ||
				strcmp( current->value, TASM_KEYWORD_RR8 ) == 0
			) {
				/*
					TOKEN:
						INT
					AST:
					        INT
						/\
					     L /  \ R
					  (NULL) (NULL)
				*/
				appendAst( createAst( current, NULL, NULL ) );
			}
		}

		current = current->next;
	}
}

void printAstStream() {
	struct ast* current = astStream;

	while ( current != NULL ) {
		char *midVal, *rightVal, *leftVal;
		token_kind_t midK, rightK, leftK;

		if ( current->mid != NULL ) {
			midVal = current->mid->value;
			midK = current->mid->kind;
		}else {
			midVal = "(NULL)";
			midK = 0x00;
		}

		if ( current->right != NULL ) {
			rightVal = current->right->value;
			rightK = current->right->kind;
		}else {
			rightVal = "(NULL)";
			rightK = 0x00;
		}

		if ( current->left != NULL ) {
			leftVal = current->left->value;
			leftK = current->left->kind;
		}else {
			leftVal = "(NULL)";
			leftK = 0x00;
		}

		printf(
			"AST:\n"
			"  AST_STREAM->MID:\n"
			"    NODE->VALUE='%s'\n"
			"    NODE->KIND='%d'\n"
			"  AST_STREAM->RIGHT:\n"
			"    NODE->VALUE='%s'\n"
			"    NODE->KIND='%d'\n"
			"  AST_STREAM->LEFT:\n"
			"    NODE->VALUE='%s'\n"
			"    NODE->KIND='%d'\n",
			midVal, midK,
			rightVal, rightK,
			leftVal, leftK
		);

		current = current->next;
	}
}

ubyte_t convertTokenToByte( char* symbol, int mode ) {
	if ( strcmp( symbol, TASM_KEYWORD_IP ) == 0 ) return REGISTER_IP;
	if ( strcmp( symbol, TASM_KEYWORD_RK ) == 0 ) return REGISTER_RK;
	if ( strcmp( symbol, TASM_KEYWORD_RC ) == 0 ) return REGISTER_RC;
	if ( strcmp( symbol, TASM_KEYWORD_SP ) == 0 ) return REGISTER_SP;
	if ( strcmp( symbol, TASM_KEYWORD_R1 ) == 0 ) return REGISTER_R1;
	if ( strcmp( symbol, TASM_KEYWORD_R2 ) == 0 ) return REGISTER_R2;
	if ( strcmp( symbol, TASM_KEYWORD_R3 ) == 0 ) return REGISTER_R3;
	if ( strcmp( symbol, TASM_KEYWORD_R4 ) == 0 ) return REGISTER_R4;
	if ( strcmp( symbol, TASM_KEYWORD_R5 ) == 0 ) return REGISTER_R5;
	if ( strcmp( symbol, TASM_KEYWORD_R6 ) == 0 ) return REGISTER_R6;
	if ( strcmp( symbol, TASM_KEYWORD_R7 ) == 0 ) return REGISTER_R7;
	if ( strcmp( symbol, TASM_KEYWORD_R8 ) == 0 ) return REGISTER_R8;
	if ( strcmp( symbol, TASM_KEYWORD_RP1 ) == 0 ) return REGISTER_RP1;
	if ( strcmp( symbol, TASM_KEYWORD_RP2 ) == 0 ) return REGISTER_RP2;
	if ( strcmp( symbol, TASM_KEYWORD_RP3 ) == 0 ) return REGISTER_RP3;
	if ( strcmp( symbol, TASM_KEYWORD_RP4 ) == 0 ) return REGISTER_RP4;
	if ( strcmp( symbol, TASM_KEYWORD_RP5 ) == 0 ) return REGISTER_RP5;
	if ( strcmp( symbol, TASM_KEYWORD_RP6 ) == 0 ) return REGISTER_RP6;
	if ( strcmp( symbol, TASM_KEYWORD_RP7 ) == 0 ) return REGISTER_RP7;
	if ( strcmp( symbol, TASM_KEYWORD_RP8 ) == 0 ) return REGISTER_RP8;
	if ( strcmp( symbol, TASM_KEYWORD_RR1 ) == 0 ) return REGISTER_RR1;
	if ( strcmp( symbol, TASM_KEYWORD_RR2 ) == 0 ) return REGISTER_RR2;
	if ( strcmp( symbol, TASM_KEYWORD_RR3 ) == 0 ) return REGISTER_RR3;
	if ( strcmp( symbol, TASM_KEYWORD_RR4 ) == 0 ) return REGISTER_RR4;
	if ( strcmp( symbol, TASM_KEYWORD_RR5 ) == 0 ) return REGISTER_RR5;
	if ( strcmp( symbol, TASM_KEYWORD_RR6 ) == 0 ) return REGISTER_RR6;
	if ( strcmp( symbol, TASM_KEYWORD_RR7 ) == 0 ) return REGISTER_RR7;
	if ( strcmp( symbol, TASM_KEYWORD_RR8 ) == 0 ) return REGISTER_RR8;
	if ( strcmp( symbol, TASM_KEYWORD_JMP ) == 0 ) return X8000_JMP;
	if ( strcmp( symbol, TASM_KEYWORD_JE ) == 0 ) return X8000_JE;
	if ( strcmp( symbol, TASM_KEYWORD_JNE ) == 0 ) return X8000_JNE;
	if ( strcmp( symbol, TASM_KEYWORD_JNZ ) == 0 ) return X8000_JNZ;
	if ( strcmp( symbol, TASM_KEYWORD_CALL ) == 0 ) return X8000_CALL;
	if ( strcmp( symbol, TASM_KEYWORD_RET ) == 0 ) return X8000_RET;
	if ( strcmp( symbol, TASM_KEYWORD_INC ) == 0 ) return X8000_INC;
	if ( strcmp( symbol, TASM_KEYWORD_DEC ) == 0 ) return X8000_DEC;
	if ( strcmp( symbol, TASM_KEYWORD_INT ) == 0 ) return X8000_INT;
	if ( strcmp( symbol, TASM_KEYWORD_MOV ) == 0 && mode == 0 ) return X8000_MOV_R;
	if ( strcmp( symbol, TASM_KEYWORD_MOV ) == 0 && mode == 8 ) return X8000_MOV_8;
	if ( strcmp( symbol, TASM_KEYWORD_MOV ) == 0 && mode == 16 ) return X8000_MOV_16;
	if ( strcmp( symbol, TASM_KEYWORD_MOV ) == 0 && mode == 32 ) return X8000_MOV_32;
	if ( strcmp( symbol, TASM_KEYWORD_MOV ) == 0 && mode == 64 ) return X8000_MOV_64;
	if ( strcmp( symbol, TASM_KEYWORD_CMP ) == 0 && mode == 0 ) return X8000_CMP_R;
	if ( strcmp( symbol, TASM_KEYWORD_CMP ) == 0 && mode == 8 ) return X8000_CMP_8;
	if ( strcmp( symbol, TASM_KEYWORD_CMP ) == 0 && mode == 16 ) return X8000_CMP_16;
	if ( strcmp( symbol, TASM_KEYWORD_CMP ) == 0 && mode == 32 ) return X8000_CMP_32;
	if ( strcmp( symbol, TASM_KEYWORD_CMP ) == 0 && mode == 64 ) return X8000_CMP_64;
	if ( strcmp( symbol, TASM_KEYWORD_ADD ) == 0 && mode == 0 ) return X8000_ADD_R;
	if ( strcmp( symbol, TASM_KEYWORD_ADD ) == 0 && mode == 8 ) return X8000_ADD_8;
	if ( strcmp( symbol, TASM_KEYWORD_ADD ) == 0 && mode == 16 ) return X8000_ADD_16;
	if ( strcmp( symbol, TASM_KEYWORD_ADD ) == 0 && mode == 32 ) return X8000_ADD_32;
	if ( strcmp( symbol, TASM_KEYWORD_ADD ) == 0 && mode == 64 ) return X8000_ADD_64;
	if ( strcmp( symbol, TASM_KEYWORD_SUB ) == 0 && mode == 0 ) return X8000_SUB_R;
	if ( strcmp( symbol, TASM_KEYWORD_SUB ) == 0 && mode == 8 ) return X8000_SUB_8;
	if ( strcmp( symbol, TASM_KEYWORD_SUB ) == 0 && mode == 16 ) return X8000_SUB_16;
	if ( strcmp( symbol, TASM_KEYWORD_SUB ) == 0 && mode == 32 ) return X8000_SUB_32;
	if ( strcmp( symbol, TASM_KEYWORD_SUB ) == 0 && mode == 64 ) return X8000_SUB_64;
	if ( strcmp( symbol, TASM_KEYWORD_MUL ) == 0 && mode == 0 ) return X8000_MUL_R;
	if ( strcmp( symbol, TASM_KEYWORD_MUL ) == 0 && mode == 8 ) return X8000_MUL_8;
	if ( strcmp( symbol, TASM_KEYWORD_MUL ) == 0 && mode == 16 ) return X8000_MUL_16;
	if ( strcmp( symbol, TASM_KEYWORD_MUL ) == 0 && mode == 32 ) return X8000_MUL_32;
	if ( strcmp( symbol, TASM_KEYWORD_MUL ) == 0 && mode == 64 ) return X8000_MUL_64;
	if ( strcmp( symbol, TASM_KEYWORD_DIV ) == 0 && mode == 0 ) return X8000_DIV_R;
	if ( strcmp( symbol, TASM_KEYWORD_DIV ) == 0 && mode == 8 ) return X8000_DIV_8;
	if ( strcmp( symbol, TASM_KEYWORD_DIV ) == 0 && mode == 16 ) return X8000_DIV_16;
	if ( strcmp( symbol, TASM_KEYWORD_DIV ) == 0 && mode == 32 ) return X8000_DIV_32;
	if ( strcmp( symbol, TASM_KEYWORD_DIV ) == 0 && mode == 64 ) return X8000_DIV_64;
	return (ubyte_t)0x00;
}

ssize_t convertNumberToBytes( char* numstr ) {
	int base = 10;
	char* endptr;

	if ( strlen( numstr ) > 2 && ( numstr[ 0 ] == '0' && ( numstr[ 1 ] == 'x' | numstr[ 1 ] == 'X' ) ) ) {
		base = 16;
	}

	return (ssize_t)strtoll( numstr, &endptr, base );
}

void tasmCodeGen() {
	programBin = (ubyte_t*)malloc( programBinCursor );
	struct ast* node = astStream;

	while ( node != NULL ) {
		if ( node->mid->kind == TASM_TOKEN_KIND_ID ) {
			appendLabel(
				createLabel( node->mid->value, programBinCursor )
			);
		}else if (
			strcmp( node->mid->value, TASM_KEYWORD_MOV ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_CMP ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_ADD ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_SUB ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_MUL ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_DIV ) == 0
		) {
			/*
			AST:
				MOV
				/\
			     L /  \ R
		             0xFF  RK
			*/

			ubyte_t rnodeByte = (ubyte_t)0x00;
			struct token* rnode = node->right;
			struct token* lnode = node->left;

			if ( rnode != NULL ) {
				rnodeByte = convertTokenToByte( rnode->value, CONVERT_TOKEN_BYTE_MODE_DEFAULT );
			}

			if ( lnode != NULL ) {
				if ( lnode->kind == TASM_TOKEN_KIND_KEYWORD ) {
					ubyte_t lnodeByte = convertTokenToByte( lnode->value, CONVERT_TOKEN_BYTE_MODE_DEFAULT );

					programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
					programBin[ programBinCursor - 1 ] = convertTokenToByte( node->mid->value, CONVERT_TOKEN_BYTE_MODE_REG );

					programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
					programBin[ programBinCursor - 1 ] = rnodeByte;

					programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
					programBin[ programBinCursor - 1 ] = lnodeByte;
				}else if ( lnode->kind == TASM_TOKEN_KIND_NUMBER ) {
					ssize_t num = convertNumberToBytes( lnode->value );
					union {
						ssize_t value;
						char byt[ 8 ];
					} numBytes;
					numBytes.value = num;

					programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
					programBin[ programBinCursor - 1 ] = convertTokenToByte( node->mid->value, CONVERT_TOKEN_BYTE_MODE_64 );

					programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
					programBin[ programBinCursor - 1 ] = rnodeByte;

					for ( int i = 0; i < 8; i++ ) {
						programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
						programBin[ programBinCursor - 1 ] = numBytes.byt[ i ];
					}
				}
			}
		}else if (
			strcmp( node->mid->value, TASM_KEYWORD_JMP ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_JE ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_JNZ ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_CALL ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_INC ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_DEC ) == 0
		) {
			/*
			AST:
				JMP
				/\
			     L /  \ R
			  (NULL) __LABEL__
			*/

			programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
			programBin[ programBinCursor - 1 ] = convertTokenToByte( node->mid->value, CONVERT_TOKEN_BYTE_MODE_DEFAULT );
			struct token* rnode = node->right;

			if ( rnode->kind == TASM_TOKEN_KIND_ID ) {
				struct label* lb = searchLabel( rnode->value );

				if ( lb == NULL ) {
					lb->pos = (size_t)0x0;
				}

				union {
					size_t value;
					char byt[ 8 ];
				} posBytes;
				posBytes.value = lb->pos;

				for ( size_t i = 0; i < 8; i++ ) {
					programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
					programBin[ programBinCursor - 1 ] = posBytes.byt[ i ];
				}
			}else if ( rnode->kind == TASM_TOKEN_KIND_KEYWORD ) {
				programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
				programBin[ programBinCursor - 1 ] = convertTokenToByte( rnode->value, CONVERT_TOKEN_BYTE_MODE_DEFAULT );
			}
		}else if (
			strcmp( node->mid->value, TASM_KEYWORD_INT ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RET ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_IP ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RK ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RC ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_SP ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R1 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R2 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R3 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R4 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R5 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R6 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R7 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_R8 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP1 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP2 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP3 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP4 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP5 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP6 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP7 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RP8 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR1 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR2 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR3 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR4 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR5 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR6 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR7 ) == 0 ||
			strcmp( node->mid->value, TASM_KEYWORD_RR8 ) == 0
		) {
			programBin = (ubyte_t*)realloc( programBin, ++programBinCursor );
			programBin[ programBinCursor - 1 ] = convertTokenToByte( node->mid->value, CONVERT_TOKEN_BYTE_MODE_DEFAULT );
		}

		node = node->next;
	}
}

void tasmCodeGenFree() {
	free( programBin );
}

void tasm_init( char* _program, size_t _programSize ) {
	program = _program;
	programSize = _programSize;
}

void tasm_free() {
	free( program );
	freeTokens();
	freeLabel();
	freeAst();
	tasmCodeGenFree();
}
// ==================== TASM ====================

// ==================== Main ====================
int main( int argc, char* argv[] ) {
	if ( argc != 4 ) {
		if ( argc == 1 ) {
			fprintf( stderr, "Error: No file specified.\n" );
		}else if ( argc < 4 ) {
			fprintf( stderr, "Error: Invalid usage.\n" );
		}else {
			fprintf( stderr, "Error: Invalid argv.\n" );
		}

		exit( EXIT_FAILURE );
	}

	if ( strcmp( argv[ 2 ], "-o" ) != 0 ) {
		fprintf( stderr, "Error: Invalid usage.\n" );
		exit( EXIT_FAILURE );
	}

	char* inputFileAddress = argv[ 1 ];
	char* outputFileAddress = argv[ 3 ];

	FILE* fptr = fopen( inputFileAddress, "rb" );
	if ( fptr == NULL ) {
		fprintf( stderr, "Error: Cannot open the specified file.\n" );
		exit( EXIT_FAILURE );
	}

	size_t fileSize = 0;
	char* file = (char*)malloc( fileSize );
	char fch;

	while ( ( fch = fgetc( fptr ) ) != EOF ) {
		file = (char*)realloc( file, ++fileSize );
		file[ fileSize - 1 ] = fch;
	}

	fclose( fptr );

	// TASM compile the assembly file
	tasm_init( file, fileSize );

	tasmLexer();
	// printTokenStream();
	tasmParser();
	// printAstStream();
	tasmCodeGen();

	// printf( "Program ...\n" );
	// for ( size_t i = 0; i < programBinCursor; i++ ) {
	// 	printf( "%d\n", programBin[ i ] );
	// }
	// printf( "Program ...\n" );

	// Write program bin to output file
	FILE* outputFilePtr = fopen( outputFileAddress, "wb" );
	if ( outputFilePtr == NULL ) {
		fprintf( stderr, "Error: Cannot create the file in specified address.\n" );
		goto out;
	}

	size_t bytesWritten = fwrite( programBin, sizeof( ubyte_t ), programBinCursor, outputFilePtr );
	if ( bytesWritten != programBinCursor ) {
		fprintf( stderr, "Error: Cannot write into the specified file.\n" );
		fclose( outputFilePtr );
		goto out;
	}

	if ( fclose( outputFilePtr ) != 0 ) {
		fprintf( stderr, "Error: Cannot close the output file.\n" );
		goto out;
	}

	out:
		tasm_free();
		exit( EXIT_SUCCESS );
}
// ==================== Main ====================
