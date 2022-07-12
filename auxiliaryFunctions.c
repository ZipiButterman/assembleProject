#include "dataBase.h"
#include "define.h"

/*A function to calc the offset to a specific label.*/
int offsetCalc(int n) 
{ 
	return (n % 16);
}

/*A function to calc the base to a specific label.*/
int baseCalc(int n) 
{ 
	return (n - (n % 16));
}

/*A function to bypasses all the white sapces.
The function gets 2 parameters:
line: the line wuth the white spaces.
i: the column in the line.*/
int whiteSpace(char *line, int i) 
{  
	while(isspace(line[i]) && line[i] != '\n')
		i++;		
	return i;
}

char *nextWord(char *line, char *word, int column)
{
	char *buf = NULL;
	int j = 0;
	buf = allocationWord(buf, strlen(line));
	strcpy(buf, line);
	while((j++) < column)
		buf++;
	word = strtok(buf, " \t \n \v \f \r");
	return word;
}

/*A function that checks for each line that the length of the line is not bigger than MAX_SIZE*/
boolean longLine(char *line) 
{ 
	int i = 0;
	while(line[i] != '\n')
		i++;
	i++;
	if(i > (MAX_SIZE - 3))
			return TRUE;
	return FALSE;
}


/*this function checks if a specific word is an action word. if so, the function returns the number of the operands the action can get, else return -1, and start to encode the opcode and funct.*/
int actions(char *word) 
{
	int i;
	for(i = 0; i < ACTION_REGISTER_NUMBER; i++) {
		if(!strcmp(word, actArr[i].name)) {
			g->code[g->indexInCodeA] = A;
			g->code[g->indexInCodeA] <<= (MOVE_ARE - actArr[i].opcode);
			g->code[g->indexInCodeA]++;
			g->code[g->indexInCodeA] <<= actArr[i].opcode;
			g->indexInCodeA++;
			g->L++;
			if(i == 14 || i == 15)
				return actArr[i].opcode;
			g->L++;
			g->code[g->indexInCodeA] = A;
			g->code[g->indexInCodeA] <<= FUNCT;
			g->code[g->indexInCodeA] +=  actArr[i].funct;
			g->code[g->indexInCodeA] <<= SOURCE_REGISTER;
			return actArr[i].opcode;
		}
	}
	return -1;
}

/*this function checks if a specific word is a directive word. if so, the function returns the number of the directive, else return -1.*/
directiveType directive(char *word) 
{
	int flag = -1;
	char* directiveArr[4] = { 
	  ".data"   , 
	  ".string" , 
	  ".entry"  , 
	  ".extern" 
	};
	int i;
	for(i = 0; i < 4; i++)
		if(!strcmp(word, directiveArr[i]))
			flag = i;
	switch(flag) {
		case 0:
			return DATA;
		case 1:
			return STRING;
		case 2:
			return ENTRY;
		case 3:
			return EXTERN;
	}
	return NONE;
}

/*this function gets a line tnd checks if it is an empty line. if so, the function returns 1 else 0*/
boolean emptyLine(char *line) 
{
	int i = 0;
	while(line[i] != '\n') {
		if(!isspace(line[i++]))
			return FALSE;
	}
	return TRUE;
}

/*This function gets a word and checks if the word is a label.
The function gets 5 paramenters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
word: the word to check.
declare: this variable signs if the word is a declaration of a label or using a label. if it is 0 - check if it is correc label but not declaration (without ':'). if it is 1 - means that this is the first word, and it is declaration of the label.
row: the number row of the line.
registers: to send it to another functions.
variables:
flag: the value to return, if flag is 0 - the word is not a label, else, the word is a label
i, j: index.*/
int isLabel(char *fileName, char *word, int declare, int row, char *registers[]) 
{
	int i = 0, j;
	char* directiveArr[4] = { 
	  "data"   , 
	  "string" , 
	  "entry"  , 
	  "extern" 
	};
	if(declare) {
		i = 2;
		if(word[strlen(word) - 1] != ':') /*a declaration must finish with ':'*/
			return 0;
		strtok(word, ":");
	}
	if(!isalpha(word[0])) { /*The first char of the label has to be letter*/
		if(declare)
			printerrors(fileName, FIRST_LETTER, row, word);
		return i;
	}
	if(strlen(word) >= LABEL_SIZE) { /*a label can't be more than 32 chars*/
		if(declare)
			printerrors(fileName, LONG_LABEL, row, word);
		return i;
	}
	if(!strcmp(word, "macro") || !strcmp(word, "endm")) {/*can't be reserved word*/
		if(declare)
			printerrors(fileName, RESERVED_WORD, row, word);
		return i;
	}
	for(j = 0; j < ACTION_REGISTER_NUMBER; j++) { /*a label can't be reserved word*/
		if(!strcmp(word, actArr[j].name)) {
			if(declare)
				printerrors(fileName, RESERVED_WORD, row, word);
			return i;
		}
	}
	for(j = 0; j < 4; j++) { /*a label can't be reserved word*/
		if(!strcmp(word, directiveArr[j])) {
			if(declare)
				printerrors(fileName, RESERVED_WORD, row, word);
			return i;
		}
	}
	for(j = 0; j < ACTION_REGISTER_NUMBER; j++) { /*a label can't be reserved word*/
		if(!strcmp(word, registers[j])) {
			if(declare)
				printerrors(fileName, RESERVED_WORD, row, word);
			return i;
		}
	}
	for(j = 1; j < strlen(word); j++) { /*a label can contain just letters and numbers*/
		if(!isalnum(word[j])) {
			if(declare)
				printerrors(fileName, ALPHANUMERIC_LABEL, row, word);
			return i;
		}
	}
	return 1;
}

char *allocationWord(char *word, int size) 
{
	 word = (char*)malloc(size);
	 if(word == NULL) {
	 	printf("Failed to allocate space to word. exiting.\n");
	 	exit(1);
	 }
	 return word;
}
