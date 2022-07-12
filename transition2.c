#include "dataBase.h"
#include "define.h"

/*This function performs the second trans and finish the encoding. the errors that revealed in this transition are: a label that defined as entry label and in practice they didn't declared and if there is a using in a label that didn't declared.
The function gets 3 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
f_am: the file to reads the lines from.
registers: to send it to "first" function.
variables:
row: the current row (using if the program needs to notify to the user about errors.)
end: gets the value returned from "first" function.
errorFlag: counts the errors.
found: sign if the a label that defined as entry really declared.
labelRow: the number of the row that there is there an entry label (using if there is a label that defined as entry label and in practice they didn't declared.)
column: the number of column in the current line. 
line: the current line to send to "first" function.
word: the label that defined as entry (using if there is a label that defined as entry label and in practice they didn't declared.)
ent_node: a pointer to entry list.
st_node: a pointer to symbolTabel list.*/
int secondTrans(char *fileName, FILE* f_am, char *registers[]) 
{
	int row = 1;
	int end = 0;
	int errorFlag = 0;
	int labelRow = 0;
	int column = 0;
	char line[MAX_SIZE];
	char word[LABEL_SIZE];
	boolean found;
	entryList *ent_node = NULL;
	symbolTabel *st_node = NULL;
	ent_node = g->ent_head;
	while(ent_node != NULL) { /*check if there is a label defined as .entry label, and in practice didn't declared.*/
		found = FALSE;
		st_node = g->st_head;
		strcpy(word, ent_node->label);
		labelRow = ent_node->row;
		while(st_node != NULL) {
			if(!strcmp(ent_node->label, st_node->symbol))
				found = TRUE;
			st_node = st_node->next;
		}
		if(found == FALSE) {
			printerrors(fileName, JUST_ENTRY, labelRow, word);
			return 1;
		}
		ent_node = ent_node->next;
	}
	while(fgets(line, MAX_SIZE - 1, f_am) != NULL) {
		column = whiteSpace(line, column); /*go to the next char that not space.*/
		if(line[column] == ';') { /*check if the line is a comment line.*/
			row++;
			continue;
		}
		if(emptyLine(line) == TRUE) { /*check if the line is an empty line.*/
			row++;
			continue;
		}
		end = second(fileName, line, row, registers); /*sends the line to "second" function.*/
		if(end) /*there is an error in this line, count the error.*/
			errorFlag++;
		row++;
	}
	return errorFlag;
}

/*This function gets a line from the "secondTrans" function and sends it to "action" function, to complete the base and offset of all the labels (if there is a using in a label that didn't declared, the program notify to the user an error and step to the next file).
The function gets 4 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
line: the line that has sent to "action" function.
row: the current row (using if the program needs to notify to the user about errors.)
registers: to send it to another functions.
variables:
column: the number of column in the current line.
isOK: gets the return value from some function - to see if there are errors in the line.
opcode: the opcode of the action (in case of action line).
ent_node: a node type "entryList". to go through the whole list with this node.
st_node: a node type "symbolTable". to go through the whole list with this node.
word1: the first word in the line.
word2: the second word in the line.*/
int second(char *fileName, char *line, int row, char *registers[]) 
{
	int column = 0;
	int isOK = 0;
	int opcode = 0;
	char *word1 = NULL;
	char *word2 = NULL;
	symbolTabel* st_node = NULL;
	entryList* ent_node = NULL;
	column = whiteSpace(line, column);
	word1 = nextWord(line, word1, column); 
	column += strlen(word1);
	if(isLabel(fileName, word1,1,row, registers) == TRUE) { /*if the word is a declaration of label.*/
		ent_node = g->ent_head;
		st_node = g->st_head;
		strtok(word1, ":");
		while(st_node != NULL) { /*find the label in the symbolTabel*/
			if(!strcmp(st_node->symbol, word1))
				break;
			st_node = st_node->next;
		}
		if(st_node == NULL) /*the label is a label before .extern ro .entry label, ignoring.*/
			return 0;
		while(ent_node != NULL) { /*copy the base and the offset from the symbolTabel to entryList*/
			if(!strcmp(ent_node->label, st_node->symbol)) {
				ent_node->base = st_node->base;
				ent_node->offset = st_node->offset;
			}
			ent_node = ent_node->next;
		}	
		column = whiteSpace(line, column);
		word2 = nextWord(line, word2, column); 
		column += strlen(word2);
		opcode = actions(word2);
		isOK += checkActionLine(fileName, 2, line, opcode, column, row, registers);
	}
	else { /*not a label*/
	
		opcode = actions(word1);
		isOK += checkActionLine(fileName, 2, line, opcode, column, row, registers);
	}
	return isOK;
}
