#include "dataBase.h"
#include "define.h"

/*This function reads a line from the input file and sends the line to the "first" function to parse the line.
The function receives 3 parameters:
fileName: The name of the current file (useful if the program needs to notify the user of errors.)
f_am: The file from which the lines are read.
Register: To send it to the "first" function.
variabels:
row: Current line number (Useful if the program needs to notify the user of errors.)
end: Gets the returned value from the "first" function.
errorFlag: Count the errors.
Column: The number of columns in the current row.
Line: The current line to be sent to the "first" function.*/
int firstTrans(char *fileName, FILE* f_am, char *registers[]) 
{
	int row = 1;
	int end = 0;
	int errorFlag = 0;
	int column = 0;
	char line[MAX_SIZE];
	while(fgets(line, MAX_SIZE - 1, f_am) != NULL) { /*read the next line*/
		column = whiteSpace(line, 0); /*go to the next char that not space.*/
		if(line[column] == ';') { /*check if the line is a comment line.*/
			row++;
			continue;
		}
		if(emptyLine(line) == TRUE) { /*check if the line is an empty line.*/
			row++;
			continue;
		}
		end = first(fileName, line, row, registers);
		if(end) /*if there is an error in this line, count the error.*/
			errorFlag++;
		row++;
	}
	end = checkEntryExtern(fileName, row); /*check if there are entry and extern label*/
	if(end)	
		errorFlag++;
	return errorFlag;
}

/*This function receives a line from the "firstTrans" function and analyzes it.
The function receives 4 parameters:
fileName: The name of the current file (useful if the program needs to notify the user of errors.)
Line: The line for analysis.
Line: Current line number (Useful if the program needs to notify the user of errors.)
Register: To send it to another function.
Change:
Column: The column number in the current row.
isDirective: To check if a specific word is a prompt word.
opcode: To check if a particular word is an action word.
dataCounter: The length / amount of data (for each data \ string command).
isOK: Gets the return value from any function - to see if there are any errors in the line.
flagWord: Indicates whether the line is a o.k.
dataFlag: Indicates whether there is a data label in the symbol table.
Word 1: The first word in a line.
Word 2: The second word in a row.
st_node: "symbolTable" pointer. To go through the entire list with this pointer.*/
int first(char *fileName, char *line, int row, char *registers[]) 
{
	int column = 0;
	int isDirective;
	int dataCounter = 0;
	int opcode;
	int isOK = 0;
	int label;
	int i = 1;
	char *word1 = NULL;
	boolean flagWord = FALSE;
	boolean dataFlag = FALSE;
	symbolTabel* st_node = NULL;
	while(line[i - 1] != '\n') {
		if(line[i] == ':' && !isspace(line[i + 1])) {
			printerrors(fileName, LABEL_SPACE, row, "");
			return 1;
		}
		i++;
	}
	column = whiteSpace(line, column);
	word1 = nextWord(line, word1, column); 
	column += strlen(word1);
	label = isLabel(fileName, word1, 1, row, registers);
	if(label == 2)
		return 1; 
	if(label == 1) { /*a declaration of label.*/
		flagWord = TRUE;
		isOK = checkLabelLine(fileName, line, word1, row, column, registers);
		if(isOK == 2)
			return 0;
		if(isOK == 1)
			return 1;
	}
	else if((isDirective = directive(word1)) != NONE) { /*first word is not a label*/
		flagWord = TRUE;
		if(isDirective == ENTRY) {
			isOK = isEntry(fileName, column, line, 0, registers, row);
			if(isOK)
				return isOK;
			return 0;
		}
		if(isDirective == EXTERN) {
			isOK = isExtern(fileName, column, line, 0, registers, row);
			if(isOK)
				return isOK;
			return 0;
		}
		if(isDirective == DATA) {
			dataCounter = isData(fileName, line, row, column);
			if(!dataCounter) {
				printerrors(fileName, ILLEGAL_DATA, row, "");
				return 1;
			}
		}
		if(isDirective == STRING)
			dataCounter = isString(fileName, line, row, column);
		if(isOK)
			return 1;
		g->DC += dataCounter; /*increase DC the length of data line.*/
		st_node = g->st_head;
		while(st_node != NULL) { /*look for first "data" label*/
			if(!strcmp(st_node->attributes, "data")) {
				dataFlag = TRUE;
				st_node->value += dataCounter;
				break;
			}
			st_node = st_node->next;
		}
		if(dataFlag == FALSE) /*No data lines, add to DCF the value of data.*/
			g->DCF += dataCounter;
	}
	else if((opcode = actions(word1)) > -1) { /*an action.*/
		flagWord = TRUE;
		if(checkActionLine(fileName, 1, line, opcode, column, row, registers))
			return 1;
		g->IC += g->L; /*increase IC the amount of information words.*/
		g->L = 0;
	}
	if(flagWord == FALSE) { /*incorrect line - ERROR*/
		if(word1[0] == '.') {
			printerrors(fileName, ILLEGAL_DIRECTIVE, row, word1);
			isOK = 1;
		}
		else {
			printerrors(fileName, ILLEGAL_COMMAND, row, word1);
			isOK = 1;
		}
	}
	return isOK; 
}


/*This function gets an extern line, and it check if it is a correct line, and enter the values to symbol tabel list. (it also checks if the labels defined already).
The function gets 6 parameters:
fileName: the name of the current file (useful if the program needs to notify to the user about errors.)
column: the number of column in the current line.
line: to read the next word from the line.
declare: This variable is passed to the "isLegalNameOfLabel" function. If it is 0 - says the word should be a label, but it is not a label statement (like an entry label statement). If it's 1 - means it's the first word, and it's the label statement.
Register: To send it to another function.
row: Current line number (Useful if the program needs to notify the user of errors.)
variables:
word2: To enter the next word there.
ent_node: points to "entryList". To go through the whole list with this pointer.
st_node: Pointer to "symbolTable". To go through the whole list with this pointer.
node: "symbolTable" type node. To add it to the list when there is a label.*/
int isExtern(char *fileName, int column, char *line, int declare, char *registers[], int row) 
{
	char *word2 = NULL;
	int label = 0;
	symbolTabel* node = NULL;
	symbolTabel* st_node = NULL;
	entryList* ent_node = NULL;
	column = whiteSpace(line, column);
	word2 = nextWord(line, word2, column); 
	column += strlen(word2);
	if(strlen(word2) == 0) { /*No label - ERROR*/
		printerrors(fileName, ILLEGAL_LINE, row, line);
		return 1;
	}
	label = isLabel(fileName, word2, declare, row, registers);
	if(label == 2) {
		printerrors(fileName, ILLEGAL_LABEL, row, word2);
		return 1; 
	}
	if(label == 1) { /*A label*/
		st_node = g->st_head;
		while(st_node != NULL) { /*check it didn't defined as a regular label*/
			if(!strcmp((st_node->symbol), word2)) {
				if(!strcmp(st_node->attributes, "external")) {
					printerrors(fileName, WARNING_EXTERN, row, word2);
					return 0;
				}
				printerrors(fileName, EXTERN_DEFINE, row, word2);
				return 1;
			}
			st_node = st_node->next;
		}
		ent_node = g->ent_head;
		while(ent_node != NULL) { /*check it didn't defined as an entry label*/
			if(!strcmp((ent_node->label), word2)) {
				printerrors(fileName, ENTRY_EXTERN, row, word2);
				return 1;
			}
			ent_node = ent_node->next;
		}
		node = allocationSymbolTabelList(node); /*the label is O.k*/
		g->st_head = addContentToSymbolTabelNode(node, word2, 0, 0, 0, "external");
	}
	else {
		printerrors(fileName, ILLEGAL_LABEL, row, word2); /*Not a label - ERROR*/
		return 1;
	}
	return 0;
}

int checkLabelLine(char *fileName, char *line, char *word1, int row, int column, char *registers[]) 
{
	int isOK = 0;
	int dataCounter = 0;
	int opcode;
	int value;
	int base;
	int offset;
	int isDirective;
	char *word2 = NULL;
	char *attributes2 = NULL;
	char *attributes = NULL;
	boolean flagWord = FALSE;
	symbolTabel* node = NULL;
	symbolTabel* st_node = NULL;
	entryList* ent_node = NULL;
	externList* ext_node = NULL;
	st_node = g->st_head;
	while(st_node != NULL) { /*declared already - ERROR.*/
		strtok(word1, ":");
		if(!strcmp((st_node->symbol), word1)) {
			printerrors(fileName, LABEL_TWICE, row, word1);
			return 1;
		}
		strcat(word1, ":");
		st_node = st_node->next;
	}
	ent_node = g->ent_head;
	while(ent_node != NULL) { /*an entry label.*/
		strtok(word1, ":");
		if(word1 != NULL && !strcmp(word1, ent_node->label)) {
			attributes2 = allocationWord(attributes2, 6);
			strcpy(attributes2, "entry");
		}
		strcat(word1, ":");
		ent_node = ent_node->next;
	}
	ext_node = g->ext_head;
	while(ext_node != NULL) { /*extern label - ERROR.*/
		strtok(word1, ":");
		if(word1 != NULL && !strcmp(word1, ext_node->label)) {
			printerrors(fileName, EXTERN_DEFINE, row, word1);
			return 1;
		}
		strcat(word1, ":");
		ext_node = ext_node->next;
	}
	column = whiteSpace(line, column);
	word2 = nextWord(line, word2, column); 
	column += strlen(word2);
	if((isDirective = directive(word2)) != NONE) {
		flagWord = TRUE;
		if(isDirective == ENTRY) {
			printerrors(fileName, WARNING_LABEL, row, line);
			isOK = isEntry(fileName, column, line, 0, registers, row);
			if(isOK)
				return isOK;
			return 2;
		}
		else if(isDirective == EXTERN) {
			printerrors(fileName, WARNING_LABEL, row, line);
			isOK = isExtern(fileName, column, line, 0, registers, row);
			if(isOK)
				return isOK;
			return 2;
		}
		else if(isDirective == STRING) {
			dataCounter = isString(fileName, line, row, column);
			attributes = allocationWord(attributes, 5);
			strcpy(attributes,"data");
			value = dataCounter;
		}
		else if(isDirective == DATA) {
			dataCounter = isData(fileName, line, row, column);
			if(dataCounter < 1) {
				printerrors(fileName, ILLEGAL_DATA, row, "");
				return 1;
			}
			attributes = allocationWord(attributes, 5);
			strcpy(attributes,"data");
			value = dataCounter;
		}
	}
	else if((opcode = actions(word2)) > -1) { /*an action.*/
		flagWord = TRUE;
		if(checkActionLine(fileName, 1, line, opcode, column, row, registers))
			return 1;
		attributes = allocationWord(attributes, 5);
		strcpy(attributes,"code");
		value = g->IC;
		base = baseCalc(g->IC);
		offset = offsetCalc(g->IC);
	}
	if(attributes != NULL && !strcmp(attributes, "data") && g->DCF) { /*label is "data" label.*/
		value += g->DCF;
		g->DCF = 0;
	}
	node = allocationSymbolTabelList(node);
	g->st_head = addContentToSymbolTabelNode(node, word1, value, base, offset, attributes);
	if(attributes2 != NULL && !strcmp(attributes2, "entry")) /*entry label.*/
		strcpy(g->st_head->attributes2, attributes2);
	g->DC += dataCounter; /*increase DC in the length of data.*/
	g->IC += g->L; /*increase IC with L (L updates in code file)*/
	g->L = 0;
	if(flagWord == FALSE) { /*incorrect line - ERROR*/
		printerrors(fileName, ILLEGAL_LINE, row, line);
		return 1;
	}
	return isOK;
}

/*This function gets an entry line, and it check if it is a correct line, and enter the values to entry list. (it also checks if the labels defined already).
The function gets 6 parameters:
fileName: the name of the current file (useful if the program needs to notify to the user about errors.)
column: the number of column in the current line.
line: to read the next word from the line.
declare: This variable is passed to the "isLegalNameOfLabel" function. If it is 0 - says the word should be a label, but it is not a label statement (like an entry label statement). If it's 1 - means it's the first word, and it's the label statement.
Register: To send it to another function.
row: Current line number (Useful if the program needs to notify the user of errors.)
variables:
word: To enter the next word there.
ent_node: points to "entryList". To go through the whole list with this pointer.
st_node: Pointer to "symbolTable". To go through the whole list with this pointer.*/
int isEntry(char *fileName, int column, char *line, int declare, char *registers[], int row) 
{
	char *word = NULL;
	int label = 0;
	symbolTabel* st_node = NULL;
	entryList* ent_node = NULL;
	column = whiteSpace(line, column);
	word = nextWord(line, word, column); 
	column += strlen(word);
	if(strlen(word) == 0) { /*No label - ERROR*/
		printerrors(fileName, ILLEGAL_LINE, row, line);
		return 1;
	}
	label = isLabel(fileName, word, declare, row, registers);
	if(label == 2)
		return 1;
	if(label == 1) { /*A label*/
		st_node = g->st_head;
		while(st_node != NULL) {
			if(!strcmp((st_node->symbol), word))
				strcpy(st_node->attributes2, "entry");
			st_node = st_node->next;
		}
		ent_node = g->ent_head;
		while(ent_node != NULL) {
			if(!strcmp(ent_node->label, word)) {
				printerrors(fileName, WARNING_ENTRY, row, word);
				return 0;
			}
			ent_node = ent_node->next;
		}
		ent_node = allocationEntryList(ent_node); /*enter the label to the entryList*/
		g->ent_head = addContentToEntryNode(ent_node, word, row);
	}
	else {
		printerrors(fileName, ILLEGAL_LABEL, row, word); /*Not a label - ERROR*/
		return 1;
	}
	return 0;
}

/*This function checks if a specific word is an action word and what action it is, and sends the word to the appropriate function in terms of number of operands to analyze the line.
The function receives 7 parameters:
fileName: The name of the current file (useful if the program needs to notify the user of errors.)
trans: Sent to the appropriate function, indicating whether it is the first or second transition.
line: The line for analysis, the line that will be sent to the appropriate function
column: The number of columns in the current row.
row: Current line (Useful if the program needs to notify the user of errors.)
opcode: to check how mwny operands the action can get.
register: To send it to another function.
variabels:
isOK: Gets the returned value from the corresponding function, to see if there are any errors.*/
int checkActionLine(char *fileName, int trans, char *line, int opcode, int column, int row, char *registers[])
{
	int isOK = 0;
	if(opcode >= 0 && opcode <= 4)
		isOK = operand2(fileName, line, column, trans, opcode, row, registers);
	if(opcode >= 5 && opcode <= 13)
		isOK = operand1(fileName, line, column, trans, opcode, row, registers);
	if(opcode >= 14 && opcode <= 15)
		isOK = operand0(fileName, line, column, trans, row);
	return isOK;
}
