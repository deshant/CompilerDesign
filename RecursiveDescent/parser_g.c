#include <stdio.h>
#include <conio.h>
#include <string.h>
char input[100];
char prod[100][100];
int pos = -1, l, st = -1;
char id, num;
void E();
void T();
void F();
void advance();
void Td();
void Ed();

void advance() {
	pos++;
	if (pos < l) {
		if (input[pos] >= '0' && input[pos] <= '9') {
			num = input[pos];
			id = '\0';
		}
		if ((input[pos] >= 'a' || input[pos] >= 'A')
				&& (input[pos] <= 'z' || input[pos] <= 'Z')) {
			id = input[pos];
			num = '\0';
		}
	}
}

void E() {
	T();
	Ed();
}

void Ed() {

	if (input[pos] == '+') {

		advance();
		T();
		strcpy(prod[++st], "+");
		Ed();
	}
	
/*	if (input[pos] == '-') {*/

/*		advance();*/
/*		T();*/
/*		strcpy(prod[++st], "-");*/
/*		Ed();*/
/*	}*/

}

void T() {
	F();
	Td();
}

void Td() {

	if (input[pos] == '*') {

		advance();
		F();
		strcpy(prod[++st], "*");
		Td();
	}
	
/*	if (input[pos] == '/') {*/

/*		advance();*/
/*		F();*/
/*		strcpy(prod[++st], "/");*/
/*		Td();*/
/*	}*/

}

void F() {
	if (input[pos] == id) {
		char ident[2] = "\0";
		ident[0] = input[pos];
		strcpy(prod[++st], ident);
		advance();
	}
	if (input[pos] == '(') {
		strcpy(prod[++st], "(");
		advance();
		E();
		if (input[pos] == ')') {
			advance();
		}
	}
	if (input[pos] == num) {
		char ident[2] = "\0";
		ident[0] = input[pos];
		strcpy(prod[++st], ident);
		advance();
	}
}
int main() {

	int i;
	printf("Enter Input String ");
	scanf("%s", input);
	l = strlen(input);
	input[l] = '$';
	advance();
	E();

	if (pos == l) {
		printf("String Accepted\n");
		for (i = 0; i <= st; i++) {
			printf("%s ", prod[i]);
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
		}
	} else {
		printf("String rejected\n");
	}
	getch();
	return 0;
}
