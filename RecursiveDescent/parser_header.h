/*
 * Header file for Recursive Descent Parser
 * 
 * by Deshant Sharma
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define LENGTH 32	// maximum length for a word
#define CHUNK 128
#define TSIZE 256	// maximum size of symbol table

// token values
#define END -1
#define KEYWORD 1
#define CONSTANT_OR_IDENTIFIER 2
#define ADD 4
#define MULT 5
#define LP 6
#define RP 7

// struct
typedef struct 
{
	char lexeme[LENGTH+1];
	int token;
	int symbol_value;
}entry;

extern entry lookahead;

// prototypes
void inputStatement(FILE*, size_t, char);
entry analyze();
void insert(char[],int);
int lookup(char[]);
bool match(int);
void advance();

void E();
void T();
void F();

/*
 * End of Header file
 */
