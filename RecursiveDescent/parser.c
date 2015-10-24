#include "parser_header.h"

int main(){
	// read input
	inputStatement(stdin, CHUNK, '\n');
	E();
	return 0;
}

void E()
{
	printf("%s",lookahead.token);
}

void T()
{
	
}

void F()
{
	
}
