#include "dataBase.h"
#include "define.h"

/*This function prints the output files. In the begining, the ".ob" file and after the ".ent", ".ext" files (only in case of need)*/
int printfiles(char *file) 
{
	FILE *f_ent;
	FILE *f_ob;
	FILE *f_ext;
	int mask;
	int i;
	int j;
	int move;
	char *ob_file = NULL;
	char *ent_file = NULL;
	char *ext_file = NULL;
	char letter[] = {'A', 'B', 'C', 'D', 'E'};
	entryList* ent_node = NULL;
	externList* ext_node = NULL;
	ob_file = allocationWord(ob_file  , sizeof(file + 4)); 
	ent_file = allocationWord(ent_file, sizeof(file + 4));
	ext_file = allocationWord(ext_file, sizeof(file + 4)); 
	strcpy(ob_file, file);
	strcpy(ent_file, file);
	strcpy(ext_file, file);
	f_ob = fopen(strcat(ob_file, ".ob"), "w+"); /*create ".ob" file*/
	fprintf(f_ob, "\t%d %d\n", g->IC - START_IC, g->DC); /*print the title to the file (IC and DC)*/
	for(i = 0; i < (g->IC - START_IC); i++) { /*print the code in groups of 4 bits, in hexadecimal.*/
		mask = 983040;
		move = 16;
		fprintf(f_ob, "%.4d " , (i + START_IC));
		for(j = 0; j < 5; j++) {	
			fprintf(f_ob, "%c%x"  , letter[j], ((g->code[i]&mask) >> move));
			if(j != 4)
				fprintf(f_ob, "-");
			mask >>= 4;
			move -= 4;
		}
		fprintf(f_ob, "\n");
	}
	for(i = 0; i < g->DC; i++) { /*print the data in groups of 4 bits, in hexadecimal*/
		mask = 983040;
		move = 16;
		fprintf(f_ob, "%.4d " , (i + g->IC));
		for(j = 0; j < 5; j++) {	
			fprintf(f_ob, "%c%x"  , letter[j], ((g->data[i]&mask) >> move));
			if(j != 4)
				fprintf(f_ob, "-");
			mask >>= 4;
			move -= 4;
		}
		fprintf(f_ob, "\n");
	}
	g->ent_head = reverseEntryList();
	ent_node = g->ent_head;
	if(ent_node != NULL) {
		f_ent = fopen(strcat(ent_file, ".ent"), "w+");
		while(ent_node != NULL) { /*there are entry labels - print them to ".ent" file*/
			fprintf(f_ent, "%s, %.4d, %.4d\n", ent_node->label, ent_node->base, ent_node->offset);
			ent_node = ent_node->next;
		}
		fclose(f_ent);
	}
	g->ext_head = reverseExternList();
	ext_node = g->ext_head;
	if(ext_node != NULL) {
		f_ext = fopen(strcat(ext_file, ".ext"), "w+");
		while(ext_node != NULL) { /*there are extern labels - print them to ".ext" file*/
			fprintf(f_ext, "%s BASE %.4d\n%s OFFSET %.4d\n\n", ext_node->label, ext_node->base, ext_node->label, ext_node->offset);
			ext_node = ext_node->next;
		}
		fclose(f_ext);
	}
	fclose(f_ob);
	return 0;
}

/*This function prints all the errors*/
int printerrors(char *fileName, int flag, int row, char *word) 
{
	word = strtok(word, "\n");
	switch(flag) {
		case NO_FILE:
			printf("No files. exit.\n");
			break;
		case ILLEGAL_COMMAND:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal name of command.\n", fileName, row, word);
			break;	
		case ILLEGAL_DIRECTIVE:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal name of directive.\n", fileName, row, word);
			break;	
		case EMPTY_FILE:
			printf("\"%s\" file is empty file.\n", fileName);
			break;
		case LABEL_SPACE:
			printf("ERROR: in \"%s\" file: line %d: after label must be a space.\n", fileName, row);
			break;
		case WARNING_EXTERN:
			printf("WARNING: in \"%s\" file: line %d: label \"%s\" already declared as .extern label. ignoring the statement.\n", fileName, row, word);
			break;
		case WARNING_ENTRY:
			printf("WARNING: in \"%s\" file: line %d: label \"%s\" already declared as .entry label. ignoring the statement.\n", fileName, row, word);
			break;
		case COMMA:
			printf("ERROR: in \"%s\" file: line %d: ',' not in place.\n", fileName, row);
			break;
		case MANY_OP:
			printf("ERROR: in \"%s\" file: line %d: too much arguments.\n", fileName, row);
			break;
		case ILLEGAL_LINE:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal line.\n", fileName, row, word);
			break;
		case WORNG_FILE:
			printf("ERROR: can't find \"%s\" file. continue to next file.\n", fileName);
			break;
		case ILLEGAL_DATA:
			printf("ERROR: in \"%s\" file: line %d: illegal data line.\n", fileName, row);
			break;
		case ILLEGAL_STRING:
			printf("ERROR: in \"%s\" file: line %d: illegal string line.\n", fileName, row);
			break;
		case NUMBER_REGIST:
			printf("ERROR: in \"%s\" file: line %d: register must be a number between 10-15.\n", fileName, row);
			break;
		case MUST_REGIST:
			printf("ERROR: in \"%s\" file: line %d: exepted register between '[]'.\n", fileName, row);
			break;
		case FIRST_LETTER:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal name for label. the first character in a label must be a letter.\n", fileName, row, word);
			break;
		case ALPHANUMERIC_LABEL:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal name for label. a label must contain alphanumeric characters only.\n", fileName, row, word);
			break;
		case ILLEGAL_LABEL:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal name for label.\n", fileName, row, word);
			break;
		case RESERVED_WORD:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal name for label. a label can't be a reserved word.\n", fileName, row, word);
			break;
		case LONG_LABEL:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal name for label. a label can't contains more than 32 characters.\n", fileName, row, word);
			break;
		case NO_MEMMORY:
			printf("ERROR: no memmory for \"%s\" file. Passes to the next file\n", fileName);
			break;
		case ILLEGAL_NUMBER:
			printf("ERROR: in \"%s\" file: line %d: the number is not in the range of the possible numbers. (the range is between %d to %d).\n", fileName, row, MIN_NUM, MAX_NUM);
			break;
		case INDEX_OPERAND:
			printf("ERROR: in \"%s\" file: line %d: excepted '[' in index addressing method.\n", fileName, row);
			break;
		case NO_OPERAND2:
			printf("ERROR: in \"%s\" file: line %d: excepted 2 arguments.\n", fileName, row);
			break;
		case NO_OPERAND1:
			printf("ERROR: in \"%s\" file: line %d: excepted 1 argument.\n", fileName, row);
			break;
		case LABEL_NOT_FOUND:
			printf("ERROR: in \"%s\" file: line %d: label \"%s\" not found.\n", fileName, row, word);
			break;
		case ILLEGAL_OPERAND:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" illegal operand.\n", fileName, row, word);
			break;
		case NOT_COMMA:
			printf("ERROR: in \"%s\" file: line %d: there is no ',' between the arguments.\n", fileName, row);
			break;
		case LABEL_TWICE:
			printf("ERROR: in \"%s\" file: line %d: \"%s\" label declared already.\n", fileName, row, word);
			break;
		case LONG_LINE:
			printf("ERROR: in \"%s\" file: line %d: line can't contain more than 80 characters.\n", fileName, row);
			break;
		case ERROR_IN_FILE:			
			printf("%d ERRORS in \"%s\" file. Can't create \"%s.ob\" file. exit.\n\n", row, word, word);
			break;
		case EXTERN_DEFINE:
			printf("ERROR: in \"%s\" file: line %d: label \"%s\" already defined as extern label.\n", fileName, row, word);
			break;	
		case ENTRY_EXTERN:
			printf("ERROR: in \"%s\" file: line %d: label \"%s\" defined as .entry and as .extern.\n", fileName, row, word);
			break;	
		case ILLEGAL_SOURCE:			
			break;
		case ILLEGAL_TARGET:			
			printf("ERROR: in \"%s\" file: line %d: target operand - \"%s\" illegal to this command.\n", fileName,row, word);
			break;
		case WARNING_LABEL:
			printf("WARNING: in \"%s\" file: in line %d: a label before .entry or .extern directive. ignoring the label.\n", fileName, row);
			break;
		case JUST_ENTRY:
			printf("ERROR: in \"%s\" file: line %d: label \"%s\" declared as .entry but did not defined.\n", fileName,row, word);
			break;	
		case ILLEGAL_REGIST:
			printf("ERROR: in \"%s\" file: line %d: illegal \"%s\" register.\n", fileName, row, word);
			break;
	}
	return 0;
}
