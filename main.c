#include "dataBase.h"
#include "define.h"

/*					*The Project*
This program is an assembler for an imaginary computer.
it gets an assembler files (".as" ending) in the command line, it checks the file - to see there are no erros and then makes the required files (".ob" ending. And if there are entry or extern labels, will be created ".ent" ending and ".ext" ending accordingly. (In case there are errors in a specific file the program will notify the user the errors - to the standart output - and step to the next file.)*/

globals *g;
/*This is an array of structs. The struct is of the actions. The first field is the name of the action, the second field is the opcode of the action, the third field is the funct of the action, the fourth field is the amount of operands that this action need to get. (The funct and the opcode will be useful to the second information word in the machine code.)*/
action actArr[ACTION_REGISTER_NUMBER] = {
    {"mov" , 0  , 0  , 2}, 
    {"cmp" , 1  , 0  , 2}, 
    {"add" , 2  , 10 , 2},
    {"sub" , 2  , 11 , 2}, 
    {"lea" , 4  , 0  , 2}, 
    {"clr" , 5  , 10 , 1},
    {"not" , 5  , 11 , 1},
    {"inc" , 5  , 12 , 1},
    {"dec" , 5  , 13 , 1},
    {"jmp" , 9  , 10 , 1},
    {"bne" , 9  , 11 , 1},
    {"jsr" , 9  , 12 , 1},
    {"red" , 12 , 0  , 1},
    {"prn" , 13 , 0  , 1},
    {"rts" , 14 , 0  , 0},
    {"stop", 15 , 0  , 0}
};
 



/*The main function. Calls to the function that scans the input files.
The variables:
registers: All the possible registers (0 - 15).
f: The current file (with the ".as" ending - the user doesn't type the file with the ending).
f_am: The file that the function sends to "macroSpan" function to print to this file the input after the span.
as_file: the name of the file with ".as" ending.
am_file: the name of the file with ".am" ending.
i: the current file (start from 1 because when argv[0] is the name of the project.
endTrans1: if it is 0, then the input is correct *after the first transition*, else - there are errors. the program notify to the user and step to the next file.
endTrans2: if it is 0, then the input is correct, else - there are errors. the program notify to the user and step to the next file.
endMacro: if it is 1, then the input is correct *after the span*, else - there are errors. the program notify to the user and step to the next file*/
int main(int argc, char *argv[]) 
{
	char *registers[ACTION_REGISTER_NUMBER] = {
	  "r0" ,
	  "r1" ,
	  "r2" ,
	  "r3" ,
	  "r4" ,  
	  "r5" ,  
	  "r6" , 
	  "r7" , 
	  "r8" ,  
	  "r9" , 
	  "r10", 
	  "r11",  
	  "r12", 
	  "r13", 
	  "r14", 
	  "r15"
	};
	FILE *f;
	FILE *f_am;
	char *as_file = NULL;
	char *am_file;
	int i = 1;
	int endMacro;
	int endTrans1;
	int endTrans2;
	if(argc == 1) { /*The user didn't enter files - ERROR*/
		printerrors("",NO_FILE, 0, ""); 
		return 0;
	}
	while(--argc) {
		/*Initializing the variables*/
		g = (globals*)malloc(sizeof(globals));
		if(g == NULL) {
			printf("Failed to allocate space to list. exiting.\n");
	 		exit(1);
	 	}
		memset(g->code, 0, sizeof(g->code));
		memset(g->data, 0, sizeof(g->data));
		g->st_head = NULL;
		g->ent_head = NULL;
		g->ext_head = NULL;
		g->IC = START_IC;
		g->DC = 0;
		g->DCF = 0;
		g->L = 0;
		endMacro = 0;
		endTrans1 = 0;
		endTrans2 = 0;
		g->indexInData = 0;
		g->indexInCodeA = 0;
		g->indexInCodeB = 0;
		as_file = allocationWord(as_file, sizeof(argv[i]) + 4); 
		am_file = allocationWord(am_file, sizeof(argv[i]) + 4); 
		strcpy(as_file, argv[i]);
		strcpy(am_file, argv[i]);
		if((f = fopen(strcat(as_file, ".as"), "r")) == NULL) { /*No file with this name -ERROR*/
			printerrors(argv[i], WORNG_FILE, 0, argv[i]);
			if(argc == 1)
				return 0;
			i++;
			continue;
		}
		f_am = fopen(strcat(am_file, ".am"), "w+");
		if((endMacro = macroSpan(argv[i], f, f_am)) == FALSE) { /*can't span the file*/
			printerrors("", ERROR_IN_FILE, 1, argv[i]);
			remove(am_file);
			i++;
			continue;
		}
		rewind(f_am);
		if((endTrans1 = firstTrans(argv[i], f_am, registers)) > 0) { /*ERROR in file*/
			printerrors("", ERROR_IN_FILE, endTrans1, argv[i]);
			i++;
			continue;
		}
		fixContentInSymbolTable(); /*finish the symbol tabel*/
		if((g->IC + g->DC) > MACHINE_CODE_SIZE) { /*the code and data is more than the max size*/
			printerrors(argv[i], NO_MEMMORY, 0, "");
			printerrors("", ERROR_IN_FILE, 1, argv[i]);
			i++;
			continue;
		}
		rewind(f_am);
		if((endTrans2 = secondTrans(argv[i], f_am, registers)) > 0) { /*ERROR in file*/
			printerrors("", ERROR_IN_FILE, endTrans2, argv[i]);
			i++;
			continue;
		}
		printfiles(argv[i]); /*O.k, create ".ob", ".ent", ".ext" files.*/
		i++;
	}
	freeSymbolTabelList();
	freeEntryList();
	freeExternList();
	fclose(f);
	fclose(f_am);
	return 0;
}
