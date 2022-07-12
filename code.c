#include "dataBase.h"
#include "define.h"

/*This function gets an operand that it first char shows that this is an operand of the directly addressing method and analyze it.
The function gets 4 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
operand: the operand to analyze.
row: the current row (using if the program needs to notify to the user about errors.)
method: to check what exactlythis function needs to do after analyze the operand.
variables:
num: the number to encode.
i: index.
mask: for negetive numbers.*/
boolean method1(char *fileName, char *operand, int method, int row) 
{		
	int num = 0;
	int i = 1;
	int mask = 0xFFFF;
	if(operand[i] == '+' || operand[i] == '-') /*check if the first operand is operator*/
		i++;
	if(operand[i] == '\0') { /*ensure that the next char is a digit*/
		printerrors(fileName, ILLEGAL_OPERAND, row, operand);
		return FALSE;
	}	
	while(operand[i] != '\0') { /*ensure that all the chars are digits*/
		if(!isdigit(operand[i++])) {
			printerrors(fileName, ILLEGAL_OPERAND, row, operand);
			return FALSE;
		}
	}
	if(method == -1) {
		g->code[g->indexInCodeA] <<= ADDRESSING_TARGET;
	}
	if(method == -1 || method == -2) {
		g->L++;
		g->indexInCodeA++;
	}
	g->code[g->indexInCodeA] = A; /*encode the number in the correct line*/
	g->code[g->indexInCodeA] <<= MOVE_ARE;
	operand[0] = ' ';
	num = atoi(operand);
	if(num < MIN_NUM || num > MAX_NUM) {
		printerrors(fileName, ILLEGAL_NUMBER, row, "");
		return FALSE;
	}
	if(num < 0)
		num &= mask;
	g->code[g->indexInCodeA] += num;
	return TRUE;
}

/*This function gets an operand that it last char shows that this is an operand of the index addressing method and analyze it.
The function gets 5 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
operand: the operand to analyze.
row: the current row (using if the program needs to notify to the user about errors.)
method: to check what exactlythis function needs to do after analyze the operand.
registers: to send it to "addressing2" function.
variables:
indaxAddressing: gets the value returned from "addressing2" function.*/
boolean method2(char *fileName, char *operand, int method, int row, char *registers[]) 
{
	int indexAddressing = 0;
	indexAddressing = addressing2(fileName, operand, row, registers); /*analyze the register.*/
	if(indexAddressing == -1)
		return FALSE;
	g->code[g->indexInCodeA] += indexAddressing; /*O.k, encode it.*/
	g->code[g->indexInCodeA] <<= ADDRESSING_TARGET;
	if(method != -2) {
		g->code[g->indexInCodeA] += 2;
		g->L += 2;
	}
	if(method == -1)
		g->indexInCodeA += 2;
	return TRUE;
}

/*This function gets a register and encode it.
The function gets 1 parameters:
j: the number of the register.*/
void method3(int j) 
{
	g->code[g->indexInCodeA] += j;
	g->code[g->indexInCodeA] <<= ADDRESSING_TARGET;
	g->code[g->indexInCodeA] += 3;
}

/*This function gets encode the label addressing method.
The function gets 1 parameter:
method: to check what exactlythis function needs to do after analyze the operand.*/
void method4(int method) 
{
	g->code[g->indexInCodeA] <<= ADDRESSING_TARGET;
	g->code[g->indexInCodeA]++;
	g->L += 2;
	if(method == -1)
		g->indexInCodeA += 2;
}

/*This function gets a line from the "actions" function and analyze it.
The function gets 5 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
actionLine: the line to analyze.
column: the number of column in the current line.
trans: sign if it is the first trans or the second.
row: the current row (using if the program needs to notify to the user about errors).*/
int operand0(char *fileName, char *actionLine, int column, int trans, int row) 
{
	if(trans == 2)
		g->indexInCodeB++;
	while(actionLine[column++] != '\n') { /*check that the line without not-white space chars.*/
		if(!isspace(actionLine[column])) {
			printerrors(fileName, MANY_OP, row, "");
			return 1;
		}
	}
	return 0;
}

/*This function gets a line from the "actions" function and analyze it.
The function gets 7 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
actionLine: the line to analyze.
column: the number of column in the current line.
trans: sign if it is the first trans or the second.
opcode: sign the opcode of the current action.
row: the current row (using if the program needs to notify to the user about errors.)
registers: to send it to another functions.
variables:
i: index.
isOK: gets the return value from some function - to see if there are errors in the line.
comma: sign ifthe last char was ','.
operand: the operand that this action gets.*/
int operand1(char *fileName, char *actionLine, int column, int trans, int opcode, int row, char *registers[])
{
	char *operand;
	char *buf = NULL;
	int j = 0;
	int isOK = 0;
	boolean comma = FALSE;
	column = whiteSpace(actionLine,column); /*go to the next not-white space char.*/
	if(actionLine[column] == ',') { /*the first cher is ',' - ERROR.*/
		printerrors(fileName, COMMA, row, "");
		return 1;
	}
	column = whiteSpace(actionLine, column);
	buf = allocationWord(buf, strlen(actionLine));
	strcpy(buf, actionLine);
	while((j++) < column)
		buf++;
	operand = strtok(buf, " , \t \n \v \f \r");
	column += strlen(operand);
	if(strlen(operand) == 0) { /*there is no operand - ERROR.*/
		printerrors(fileName, NO_OPERAND1, row, "");
		return 1;
	}
	while(actionLine[column] != '\n') { /*check some errors (like unnecessary ',')*/
		if(actionLine[column] == ',') {
			comma = TRUE;
			column++;
			if(actionLine[column] == '\n') {
				printerrors(fileName, COMMA, row, actionLine);
				return 1;
			}
		}
		if(!isspace(actionLine[column])) {
			if(comma == TRUE)
				printerrors(fileName, MANY_OP, row, "");
			else
				printerrors(fileName, ILLEGAL_LINE, row, actionLine);
			return 1;
		}
		column++;
	}
	if(comma == TRUE) {
		printerrors(fileName, COMMA, row, actionLine);
		return 1;
	}
	if(trans == 1) {
		g->code[g->indexInCodeA] <<= ADDRESSING_SOURCE; /*there is no source operand*/
		g->code[g->indexInCodeA] <<= TARGET_REGISTER;
		isOK = checkOperand(fileName, operand, opcode, row, registers); /*check operand*/
	}
	else
		isOK = completeBaseOffset(fileName, operand, NULL, row, registers); /*encode base and offset*/
	return isOK;
}

/*This function gets a line from the "actions" function and analyze it.
The function gets 7 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
actionLine: the line to analyze.
column: the number of column in the current line.
trans: sign if it is the first trans or the second.
opcode: sign the opcode of the current action.
row: the current row (using if the program needs to notify to the user about errors.)
registers: to send it to another functions.
variables:
i: index.
isOK: gets the return value from some function - to see if there are errors in the line.
comma: sign ifthe last char was ','.
operand: an array of operands (for the 2 operands of the action).*/
int operand2(char *fileName, char *actionLine, int column, int trans, int opcode, int row, char *registers[])
{
	char *operand1;
	char *operand2;
	char* buf = NULL;
	int j = 0;
	int isOK = 0;
	boolean comma = FALSE;
	column = whiteSpace(actionLine,column);	/*go to the next not-white space char.*/
	if(actionLine[column] == ',') { /*the first char is ',' - ERROR.*/
		printerrors(fileName, COMMA, row, "");
		return 1;
	}
	buf = allocationWord(buf, strlen(actionLine));
	strcpy(buf, actionLine);
	while((j++) < column)
		buf++;
	operand1 = strtok(buf, " , \t \n \v \f \r");
	column += strlen(operand1);
	 if(strlen(operand1) == 0) { /*there is no operand - ERROR.*/
		printerrors(fileName, NO_OPERAND2, row, "");
		return 1;
	}
	column = whiteSpace(actionLine,column); /*go to the next not-white space char.*/
	if(actionLine[column] == '\n') { /*there is no operand - ERROR.*/
		printerrors(fileName, NO_OPERAND2, row, "");
		return 1;
	}	
	if(actionLine[column] != ',') { /*the next char is not ',' - ERROR.*/
		printerrors(fileName, NOT_COMMA, row, "");
		return 1;
	}	
	column++;
	column = whiteSpace(actionLine,column); /*go to the next not-white space char.*/
	if(actionLine[column] == ',') { /*the next char is ',' - ERROR.*/
		printerrors(fileName, COMMA, row, "");
		return 1;
	}
	j = 0;
	buf = allocationWord(buf, strlen(actionLine));
	strcpy(buf, actionLine);
	while((j++) < column)
		buf++;
	operand2 = strtok(buf, " , \t \n \v \f \r");
	column += strlen(operand2);
	while(actionLine[column] != '\n') { /*check some errors (like unnecessary)*/
		if(actionLine[column] == ',') {
			comma = TRUE;
			column++;
			if(actionLine[column] == '\n') {
				printerrors(fileName, COMMA, row, actionLine);
				return 1;
			}

		}
		if(!isspace(actionLine[column])) {
			if(comma == TRUE)
				printerrors(fileName, MANY_OP, row, "");
			else
				printerrors(fileName, ILLEGAL_LINE, row, actionLine);
			return 1;
		}
		column++;
	}
	if(comma == TRUE) {
		printerrors(fileName, COMMA, row, actionLine);
		return 1;
	}
	if(trans == 1)
		isOK = checkOperands(fileName, operand1, operand2, opcode, row, registers); /*check operands*/
	else 
		isOK = completeBaseOffset(fileName, operand1, operand2, row, registers); /*encode base offset*/
	return isOK;
}

/*This function gets a numbers to encode.
The function gets 3 parameters:
n: the number (R ro E - depend in the definition of the label) to encode.
base: the base of the label.
offset: the ofsset of the label.*/
void addRE(int n, int base, int offset) 
{
	g->code[g->indexInCodeB] = n;
	g->code[g->indexInCodeB] <<= MOVE_ARE;
	g->code[g->indexInCodeB++] += base;
	g->code[g->indexInCodeB] = n;
	g->code[g->indexInCodeB] <<= MOVE_ARE;
	g->code[g->indexInCodeB++] += offset;
}

/*This function gets an operand, if the operand contains a label - the function encode it accordingly the base and offset of the labels.
The function gets 4 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
operand: the operand to test.
row: the current row (using if the program needs to notify to the user about errors.)
registers: to send it to another functions.
variables:
oplabel: the label in the operand.
op: the the current operand.
flag: to sign if the label is in the symbol tabel.
base: the base of the label.
offset: the offset of the label.
j, i: index.
num: the number of operands.
isOK: gets the return value from some function - to see if there are errors in the line.
flagLabel: there is a flag in the operand, enter the base and offset of the operand to the list.
ext_node: a node type "externList". to go through the whole list with this node.
st_node: a node type "symbolTable". to go through the whole list with this node.*/
int completeBaseOffset(char *fileName, char *operand1, char *operand2, int row, char *registers[]) 
{
	char oplabel[LABEL_SIZE];
	char *op = operand1;
	int offset = 0;
	int base = 0;
	int j = 0;
	int i;
	int isOK = 0;
	int num = 1;
	boolean flag = FALSE;
	boolean flagLabel = FALSE;
	symbolTabel* st_node = NULL;
	externList* ext_node = NULL;
	g->indexInCodeB += 2;
	if(operand2 != NULL)
		num = 2;
	for(i = 0; i < num; i++) {
		if(op[0] == '#') /*the firs addressing method*/
			g->indexInCodeB++;
		if(isLabel(fileName, op, 0, row, registers) == TRUE) { /*addressing method contains a label*/
			flagLabel = TRUE;
			strcpy(oplabel, op);
		}
		if(op[strlen(op) - 1] == ']') { /*addressing method contains a label*/
			while(op[j] != '[') {
				oplabel[j] = op[j];
				j++;
			}
			oplabel[j] = '\0';
			j = 0;
			flag = FALSE;
			flagLabel = TRUE;
		}
		if(flagLabel == TRUE) { /*check if the label is in the symbol tabel, if not - ERROR*/
			st_node = g->st_head;
			while(st_node != NULL) {
				if(!strcmp((st_node->symbol), oplabel)) {
					base = st_node->base;
					offset = st_node->offset;
					flag = TRUE;
					break;
				}
				st_node = st_node->next;
			}
			if(flag == FALSE) {
				printerrors(fileName, LABEL_NOT_FOUND, row, oplabel);
				return 1;
			}
			if(!strcmp((st_node->attributes), "external")) { /*label is extern label*/
				ext_node = allocationExternList(ext_node); /*put the label in extern list*/
				g->ext_head = addContentToExternNode(ext_node, st_node->symbol);
				addRE(E, base, offset); /*encode the label*/
			}
			else /*label is not extern label*/
				addRE(R, base, offset); /*encode the label*/
		}
		flagLabel = FALSE;
		memset(oplabel, '\0', LABEL_SIZE);
		if(operand2 != NULL)
			strcpy(op, operand2);
	}
	return isOK;
}

/*This function gets an operand from "method2" function, isolates the register and analyze it.
The function gets 4 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
operand: the operand to analyze.
row: the current row (using if the program needs to notify to the user about errors.)
registers: to send it to another functions.
variables:
i, j: index.
flag: the number of the register.
regist: the register in the operand will be copied to regist.*/
int addressing2(char *fileName, char *operand, int row, char *registers[]) 
{
	int i = 0;
	int j = 0;
	int flag = -1;
	char regist[4];
	while(operand[i] != '[' && i < strlen(operand)) /*skip the label*/
		i++;
	if(i == strlen(operand)) { /*no '[' in the operand*/
		printerrors(fileName, ILLEGAL_OPERAND, row, operand);
		return -1;
	}
	i++;
	while(operand[i] != ']' && !isspace(operand[i]))
		regist[j++] = operand[i++];
	regist[j] = '\0';
	if(regist[0] == '\0') { /*no register - ERROR*/
		printerrors(fileName, MUST_REGIST, row, operand);
		return -1;
	}
	for(j = 0; j < ACTION_REGISTER_NUMBER; j++) {
		if(!strcmp(regist, registers[j])) {
			if(j < 10) { /*register smaller than 10 - ERROR*/
				printerrors(fileName, NUMBER_REGIST, row, regist);
				return -1;
			}
			flag = j;
		}
	}
	if(flag == -1) { /*not correct register*/
		printerrors(fileName, ILLEGAL_REGIST, row, regist);
		return -1;
	}
	return flag;
}

/*This function gets an operand and check which addressing method it.
The function gets 5 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
operand: the operand to analyze.
opcode: the opcode of the action.
row: the current row (using if the program needs to notify to the user about errors.)
registers: to send it to another functions.
variables:
flag: sign if the addressing method found (if so, flag will be false).
error: check if there is an error.
j: index.*/
int checkOperand(char *fileName, char *operand, int opcode, int row, char *registers[]) 
{
	boolean flag = TRUE;
	int error = 0;
	int j;
	if(operand[0] == '#') {
		if(opcode != 13) { /*addressing method just for "prn" action (in the actions that get 1 operand)*/
			printerrors(fileName, ILLEGAL_TARGET, row, operand);
			return 1;
		}
		if(method1(fileName, operand, -1, row) == FALSE) /*incorrect addressing method*/
			return 1;
		flag = FALSE;
	}
	if(flag == TRUE && operand[strlen(operand) - 1] == ']') {
		if(method2(fileName, operand, -1, row, registers) == FALSE) /*incorrect addressing method*/
			return 1;
		flag = FALSE;
	}
	for(j = 0; j < ACTION_REGISTER_NUMBER; j++) {
		if(flag == TRUE && !strcmp(operand, registers[j])) {
			if(opcode == 9) {
				printerrors(fileName, ILLEGAL_TARGET, row, operand);
				return 1;
			}
			method3(j); /*encode register*/
			flag = FALSE;
		}
	}
	if(flag == TRUE && isLabel(fileName, operand, 0, row, registers) == TRUE) {
		method4(-1);
		flag = FALSE;
	}
	j = 0;
	if(flag == TRUE) {
		while(operand[j] != '\0') {
			if(operand[j] == '[') {
				printerrors(fileName, INDEX_OPERAND, row, operand);
				return 1;
			}
			j++;
		}
		printerrors(fileName, ILLEGAL_OPERAND, row, operand);
		return 1;
	}
	g->indexInCodeA++;

	return error;
}

/*This function gets 2 operands and check which addressing method them.
The function gets 4 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
operand1: the first operand to check.
operand2: the second operand to check.
opcode: the opcode of the action
row: the current row (using if the program needs to notify to the user about errors.)
registers: to send it to another functions.
variables:
flag: sign if the addressing method found (if so, flag will be false).
go: sign that the first operand contains a label (keep 2 places for the base and offset).
directly: sign that the first operand is the first addressing method.
reg: sign that the first operand is a register.
error: check if there is an error.
j: index.*/
int checkOperands(char *fileName, char *operand1, char *operand2, int opcode, int row, char *registers[])
{
	boolean flag = TRUE;
	boolean go = FALSE;
	boolean directly = FALSE;
	boolean reg = FALSE;
	int error = 0;
	int j;
	if(operand1[0] == '#') {
		g->code[g->indexInCodeA] <<= ADDRESSING_SOURCE;
		if(opcode == 4) { /*addressing method not for "lea" action (in actions that get 2 operands)*/
			printerrors(fileName, ILLEGAL_SOURCE, row, operand1);
			return 1;
		}
		directly = TRUE; /*first addressing method*/
		flag = FALSE;
	}
	for(j = 0; j < ACTION_REGISTER_NUMBER; j++) {
		if(flag == TRUE && !strcmp(operand1, registers[j])) {
			if(opcode == 4) { /*addressing method not for "lea" action (in actions that get 2 operands)*/
				printerrors(fileName, ILLEGAL_SOURCE, row, operand1);
				return 1;
			}
			method3(j);
			flag = FALSE;
			reg = TRUE; /*register addressing method*/
		}
	}
	if(flag == TRUE && operand1[strlen(operand1) - 1] == ']') {
		if(method2(fileName, operand1, 2, row, registers) == FALSE) /*incorrect addressing method*/
			return 1;
		go = TRUE; /*addressing method contains label*/
		flag = FALSE;
	}
	if(flag == TRUE && isLabel(fileName, operand1, 0, row,registers) == TRUE) {
		method4(4);
		flag = FALSE;
		go = TRUE; /*addressing method contains label*/
	}
	g->code[g->indexInCodeA] <<= TARGET_REGISTER;
	j = 0;
	if(flag == TRUE) {
		while(operand1[j] != '\0') {
			if(operand1[j] == '[') {
				printerrors(fileName, INDEX_OPERAND, row, operand1);
				return 1;
			}
			j++;
		}
		printerrors(fileName, ILLEGAL_OPERAND, row, operand1);
		return 1;
	}
	flag = TRUE;
	for(j = 0; j < ACTION_REGISTER_NUMBER; j++) {
		if(flag == TRUE && !strcmp(operand2, registers[j])) {
			method3(j);
			if(directly == TRUE) { /*first operand is first addressing method*/
				if(method1(fileName, operand1, -2, row) == FALSE) /*incorrect addressing method*/
					return 1;
				directly = FALSE;
			}
			flag = FALSE;
		}
	}
	if(flag == TRUE && operand2[0] == '#') {
		if(opcode != 1) { /*addressing method just for "cmp" action (in the actions that get 2 operands)*/
			printerrors(fileName, ILLEGAL_TARGET, row, operand2);
			return 1;
		}
		g->L++;
		g->code[g->indexInCodeA] <<= ADDRESSING_TARGET;
		if(go == TRUE) { /*first operand is addressing method contains label*/
			g->indexInCodeA +=3;
			go = FALSE;
		}
		if(reg == TRUE) { /*first operand is registers addressing method*/
			g->indexInCodeA++;
			reg = FALSE;
		}
		if(directly == TRUE) { /*first operand is first addressing method*/
			g->L++;
			g->indexInCodeA++;
			if(method1(fileName, operand1, -3, row) == FALSE) /*incorrect addressing method*/
				return 1;
			directly = FALSE;
			g->indexInCodeA++;
		}
		if(method1(fileName, operand2, -3, row) == FALSE) /*incorrect addressing method*/
			return 1;
		flag = FALSE;
	}
	if(flag == TRUE && operand2[strlen(operand2) - 1] == ']') {
		if(method2(fileName, operand2, -2, row, registers) == FALSE) /*incorrect addressing method*/
			return 1;
		g->code[g->indexInCodeA] += 2;
		if(directly == TRUE) { /*first operand is first addressing method*/
			if(method1(fileName, operand1, -2, row) == FALSE) /*incorrect addressing method*/
				return 1;
			directly = FALSE;
		}
		g->indexInCodeA += 2;
	    g->L += 2;
		flag = FALSE;
	}
	if(flag == TRUE && isLabel(fileName, operand2, 0, row, registers) == TRUE) {	
		g->code[g->indexInCodeA] <<= ADDRESSING_TARGET;
		g->code[g->indexInCodeA]++;
		flag = FALSE;
		if(directly == TRUE) { /*first operand is first addressing method*/
			if(method1(fileName, operand1, -2, row) == FALSE) /*incorrect addressing method*/
				return 1;
			directly = FALSE;
		}
		g->indexInCodeA += 2;
		g->L += 2;
	}
	if(go == TRUE) { /*first operand is addressing method contains label*/
		g->indexInCodeA += 2;
		go = FALSE;
	}
	j = 0;
	if(flag == TRUE) {
		while(operand2[j] != '\0') {
			if(operand2[j] == '[') {
				printerrors(fileName, INDEX_OPERAND, row, operand2);
				return 1;
			}
			j++;
		}
		printerrors(fileName, ILLEGAL_OPERAND, row, operand2);
		return 1;
	}
	g->indexInCodeA++;
	return error;
}
