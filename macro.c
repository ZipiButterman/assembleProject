#include "dataBase.h"
#include "define.h"

/*This function gets the input file and spans all the macros.
The function gets 3 paramenters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
f: the sorce file - to span it.
f_am: the target file - to print after the spaning.
variables:
flagPrint: sign if to print the currenf line (the lines that don't have to be printed are: "macro", "endm", and line that calls to macro).
macroFlag: sign if the current line is inside "macro".
macroCounter: sign the number of the current macro line (the maximum number of the lines is 6)
lineCounter: sign the end number of lines in the current macro.
endm: sign that the macro is finish.
call: sign that this line calls to a macro, if so - don't print the current line, print instead the macro that in the macro list.
empty: counts the empty lines.
row: the current row (using to check if the file is empty file.)
isOK: gets the value returned from "action" function, to see if there are errros.
str1: the first word in the line.
str2: the second word in the line.
line: the current line to analyze.
m_head: the head of the macroList.
m_node: a node type "macroList". to go through the whole list with this node.
node: a node type "macroList". to add it to the list when there is a macro.*/
boolean macroSpan(char *fileName, FILE *f, FILE *f_am) 
{
	boolean flagPrint = TRUE;
	boolean macroFlag = FALSE;
	boolean endm = FALSE;
	boolean call;
	boolean isOK = TRUE;
	int macroCounter = 0;
	int lineCounter = 0;
	int empty = 0;
	int row = 0;
	int i;
	char str1[MAX_SIZE];
	char str2[MAX_SIZE];
	char line[MAX_SIZE];
	macroList *m_head = NULL;
	macroList *m_node;
	macroList *node = NULL;
	while(fgets(line, MAX_SIZE - 1, f) != NULL) { /*read the next line*/
		row++;
		call = TRUE;
		if(emptyLine(line) == TRUE) { /*the line is empty line*/
			fprintf(f_am, "%s", line); /*prints the line to the target file*/
			empty++; /*counts the line*/
			continue;
		}
		if(longLine(line) == TRUE) { /*the line is too long - ERROR*/
			printerrors(fileName, LONG_LINE, row, line);
			isOK = FALSE;
			continue;
		}
		sscanf(line, "%s %s", str1, str2); /*read the next 2 words*/
		if(!strcmp(str1, "endm")) { /*the line is end macro line*/
			macroCounter = 0;
			flagPrint = TRUE;
			macroFlag = FALSE;
			endm = TRUE;
			continue;
		}
		if(macroFlag == TRUE) { /*the line is inside macro*/
			lineCounter++;
			node = addLine(macroCounter, line, node); /*add the line to the macro list*/
			macroCounter++;
			continue;
		}
		if(endm == TRUE){ /*end macro - add the node to the first place in the list*/
			m_head = addFirstToMacro(node, m_head, lineCounter);
			endm = FALSE;
		}
		m_node = m_head;
		while (m_node != NULL)  { /*go throw the macro list*/
			if(!strcmp(str1,(m_node->name))) { /*the line is a calling to macro*/
				call = FALSE;
				for(i = 0; i < (m_node->numOfLines); i++) /*print to the target file the macro node*/
					fprintf(f_am, "%s", (m_node->macroLines[i]));
			}
			m_node = m_node->next;
		}
		if(!strcmp(str1, "macro")) { /*the line is a start macro line*/
			node = allocationMacroList(node); /*allocation place to macroList list*/
			node = addName(str2, node);
			flagPrint = FALSE;
	    	macroFlag = TRUE;
			continue;	    	
		}
		if(flagPrint == TRUE && call == TRUE) /*if the line has to be printed - print it to the target file.*/
			fprintf(f_am, "%s", line);
	}
	freeMacroList(m_head);
	if(empty == row) { /*if the file is an empty file step to the next file.*/
		printerrors(fileName, EMPTY_FILE, 0, "");
		return 0;
	}
	return isOK;
}
