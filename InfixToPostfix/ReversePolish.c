#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define LENGTH 32	// maximum length for a operand
#define CHUNK 128
#define STACK_SIZE 128
#define INP_SIZE 128	// max no. of symbols in input
#define OPERATOR 1
#define OPRAND 2
#define NUMBER 3

typedef enum {	err, plus, minus, mult, divide, modulo, power } operators;

typedef struct{
	char lexeme[LENGTH+1];
	int type;
	float sym_value;
}symbols;

symbols stack[STACK_SIZE];
symbols postfix[INP_SIZE];	// final result
symbols input[INP_SIZE];		// generated during validation
int top = -1, token_index = -1, postfix_index = -1;

void insertTk(char* tk, int type){
		strcpy(input[++token_index].lexeme,tk);
		input[token_index].type = type;
		if (type == OPRAND)
		{
			printf("enter value for %s: ",tk);
			scanf("%f",&input[token_index].sym_value);
		}
		else if(type == NUMBER){
			input[token_index].sym_value = (float)atoi(tk);
		}
}
bool is_identifier(char ch) {
	return ((isalpha(ch)||ch=='_'||isdigit(ch)) ? true : false);
}
bool is_operator(char ch) {
	return ((ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='%'|| ch == '^') ? true : false);
}

operators get_op_enum(char op){
	switch(op){
		case '+':
			return plus;
		case '-':
			return minus;
		case '*':
			return mult;
		case '/':
			return divide;
		case '%':
			return modulo;
		case '^':
			return power;
		default:
			return err;
	}
}

bool precedence(char* st_top,char* in_op){
	if(st_top[0] == '(' || in_op[0] == '(' ){
		printf("found op_brac\n");
		return false;
	}
	if(in_op[0] == ')')
		return true;
	operators top = get_op_enum(st_top[0]);
	operators op = get_op_enum(in_op[0]);
	return ((top >= op) ? true : false);
}
void push(symbols tk){
	strcpy(stack[++top].lexeme,tk.lexeme);
	stack[top].type = tk.type;
	stack[top].sym_value = tk.sym_value;
}
symbols pop(){
	symbols tk;
//	printf("pop: %d\n",top);
	return((top > -1) ? stack[top--] : tk);
}
void clear_Stack(){
	top = -1;
}

char* inputStatement(size_t size, char delimiter);
bool validate(char* input);
void generate_postfix();
void add_to_postfix(symbols);
void evaluate();

int main(int argc, char* argv[])
{	
    if (argc != 1)
    {
        printf("Usage: ReversePolish\n");
        return 1;
    }
    char* infix = inputStatement(CHUNK,'\n');
    int i;
	if(!validate(infix)){
		printf("invalid input\n");
	}
	else{
		generate_postfix();
		for(i = 0; i <= postfix_index; i++){
			printf("%s ",postfix[i].lexeme);
		}
		clear_Stack();
		evaluate();
	}	
    return 0;
}

// input expression
char* inputStatement(size_t size, char delimiter)
{
    char* str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char) * size);	//size is start size
    if(!str)
    	return str;
    	
    while(EOF!=(ch=fgetc(stdin)) && ch != delimiter){
    	if(ch == '\t')
    		continue;
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char) * (size+=32));
            if(!str)return str;
        }
    }
    if(ch == EOF && len == 0)
    {
    	str[0] = EOF;
    	return str;
    }
    str[len++]='\0';
    return realloc(str, sizeof(char) * len);
}

// validate expression
bool validate(char* input)
{
	int i = 0,pt = 0, oprandc = 0, operatorc=0;
	bool flag = false;
	char buffer[LENGTH+1];
	int count = strlen(input);
	for (i = 0; i < count; i += 1)
	{
		if(input[i] == '('){
			pt++;
			insertTk("(",OPERATOR);
			continue;
		}
		else if(input[i] == ')'){
			pt--;
			insertTk(")",OPERATOR);
			continue;
		}
		if(pt < 0)
			return false;
				
		// operators
		if(is_operator(input[i]))
		{
			if(is_operator(input[i+1]))
				return false;
			if(oprandc == 0)
				return false;
			else if(oprandc < operatorc)
				return false;
			operatorc++;
			buffer[0] = input[i];
			buffer[1] = '\0';
			insertTk(buffer,OPERATOR);
		}
		// capture identifier
		else if(isalpha(input[i])){
			int j = 0;
			do{
				buffer[j] = input[i];
				j++;
				i++;
			}while(is_identifier(input[i]));
			buffer[j] = '\0'; i--;
			oprandc++;
			insertTk(buffer,OPRAND);
		}
		// test numbers
		else if(isdigit(input[i])){
			int j = 0;
			do{
				buffer[j] = input[i];
				j++; i++;
				if(input[i+1] != '\0' && !isdigit(input[i]) && !is_operator(input[i])) {
					printf("lexical error, invalid integer at %d\n",i-j);
					return false;
				}
			}while(isdigit(input[i]));	
			buffer[j] = '\0'; i--;
			oprandc++;
			insertTk(buffer,NUMBER);
		}
	}
	// final test
	if(pt == 0 && oprandc == operatorc+1)
		return true;
	else
		return false;
}
// generate postfix
void generate_postfix(){
	if(token_index < 0)
		return;
	int i;
	for (i = 0; i <= token_index; i += 1)
	{
		if(input[i].type == OPRAND || input[i].type == NUMBER){
			add_to_postfix(input[i]);
//			printf("id:add_to_postfix\n");
			continue;
		}
		else{
			bool flag = false;
			while(top > -1 && precedence(stack[top].lexeme,input[i].lexeme)){
				add_to_postfix(pop());
//				printf("true:pop and add_to_postfix\n");
				flag = true;
			}
			if( input[i].lexeme[0] != ')'){
				push(input[i]);
//				printf("push- %d\n",top);
			}
			else{
				pop();
//				printf("pop - %d\n",top);
			}	
		}
	}
	while(top > -1){
		add_to_postfix(pop());
	}
}
// add to postfix
void add_to_postfix(symbols tk){
	strcpy(postfix[++postfix_index].lexeme,tk.lexeme);
	postfix[postfix_index].type = tk.type;
	postfix[postfix_index].sym_value = tk.sym_value;
}
void evaluate(){
	top = -1;int i;
	for(i = 0; i <= postfix_index; i++){
		if(postfix[i].type == OPRAND || postfix[i].type == NUMBER){
			push(postfix[i]);
		}
		else{
			symbols opr1 = pop();
			symbols opr2 = pop();
			symbols res;
			res.type = NUMBER;
			switch((int)get_op_enum(postfix[i].lexeme[0])){
				case plus:
					res.sym_value = opr2.sym_value + opr1.sym_value;
					push(res);
					break;
				case minus:
					res.sym_value = opr2.sym_value - opr1.sym_value;
					push(res);
					break;
				case mult:
					res.sym_value = opr2.sym_value * opr1.sym_value;
					push(res);
					break;
				case divide:
					res.sym_value = opr2.sym_value / opr1.sym_value;
					push(res);
					break;
				case modulo:
					res.sym_value = (int)(opr2.sym_value) % (int)(opr1.sym_value);
					push(res);
					break;
				case power:
					res.sym_value = pow(opr2.sym_value,opr1.sym_value);
					push(res);
					break;
				default:
					break;
			}
		}
	}
	symbols output = pop();
	printf("\noutput: %f\n",output.sym_value);
}
