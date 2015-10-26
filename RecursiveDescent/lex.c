#include "parser_header.h"

entry symbol_table[TSIZE];
int last_index = -1;
char buffer[LENGTH+1];
static char* statement;

// stores lookahead token
entry lookahead;

// keeps track of input
int statement_index = 0;

bool is_identifier(char ch) {
	return ((isalpha(ch)||ch=='_'||isdigit(ch)) ? true : false);
}
bool is_operator(char ch) {
	return ((ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='%') ? true : false);
}

// read statement from file (1 statement at a time)
void inputStatement(FILE* fp, size_t size, char delimiter)
{
    char* str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char) * size);	//size is start size
    if(!str){
	  	statement = str;
	  	return;
    }

    	
    while(EOF!=(ch=fgetc(fp)) && ch != delimiter){
    	if(ch == '\t')
    		continue;
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char) * (size+=32));
            if(!str){
      	    	statement = str;
            	return;
            }
        }
    }
    if(ch == EOF && len == 0)
    {
    	str[0] = EOF;
    	statement = str;
    	return;
    }
    str[len++]='\0';
    statement = realloc(str, sizeof(char) * len);
}

// analyze input statement and return analyzed token
entry analyze()
{
	entry symbol;
	symbol.token = END;
	
	// found end
	if(statement[statement_index] == '\0'){
		symbol.lexeme[0] = '\0';
		symbol.token = END;
		return symbol;
	}	
	
	switch(statement[statement_index]){
		case ' ':
			statement_index++;
			break;
		case ';':
			symbol.lexeme[0] = ';';
			symbol.lexeme[1] = '\0';
			symbol.token = END;
			return symbol;	
		case '(':
		case ')':
			symbol.lexeme[0] = statement[statement_index++];
			symbol.lexeme[1] = '\0';
			symbol.token = (statement[statement_index-1] == '(') ? LP : RP;
			return symbol;
		case '+':
			symbol.lexeme[0] = statement[statement_index++];
			symbol.lexeme[1] = '\0';
			symbol.token = ADD;
			return symbol;
		case '*':
			symbol.lexeme[0] = statement[statement_index++];
			symbol.lexeme[1] = '\0';
			symbol.token = MULT;
			return symbol;
		default:
			break;
	}

	// found number
	if(isdigit(statement[statement_index])){
		int j = 0;
		do{
			symbol.lexeme[j] = statement[statement_index];
			j++;
			statement_index++;
			if(statement[statement_index+1] != '\0' && !isdigit(statement[statement_index]) && !is_operator(statement[statement_index])){
				printf("lexical error at %d\n",statement_index+1);
				symbol.lexeme[0] = '\0';
				symbol.token = END;
				return symbol;
			}
		}while(isdigit(statement[statement_index]));
		
		symbol.lexeme[j] = '\0';
		if(lookup(symbol.lexeme) == -1){
			insert(symbol.lexeme,CONSTANT_OR_IDENTIFIER);
		}
		symbol.token = CONSTANT_OR_IDENTIFIER;
		return symbol;
	}
	// found identifier
	else if(isalpha(statement[statement_index])){
		int j = 0;
		do{
			symbol.lexeme[j] = statement[statement_index];
			j++;
			statement_index++;
		}while(is_identifier(statement[statement_index]));
		
		symbol.lexeme[j] = '\0';
		int t = lookup(symbol.lexeme);
		if(t == -1){
			insert(symbol.lexeme,CONSTANT_OR_IDENTIFIER);
		}
		else if(t == CONSTANT_OR_IDENTIFIER){
		}
		symbol.token = CONSTANT_OR_IDENTIFIER;
		return symbol;
	}	
	else
		return symbol;
}
// insert into symbol_table
void insert(char* lex,int tk){
	strcpy(symbol_table[++last_index].lexeme,lex);
	symbol_table[last_index].token = tk;
}
// lookup from symbol_table
int lookup(char* id){
	int i;
	for (i = 0; i <= last_index; i += 1)
		if(!strcmp(id,symbol_table[i].lexeme))
			return symbol_table[i].token;
	return -1;
}
