#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_SIZE 83 /*maximum length of line*/
#define MAX_NUM 65535 /*maximun number for 16 bits*/
#define MIN_NUM -65535 /*nimimum number for 16 bits*/
#define LABEL_SIZE 34 /*maximum length of label*/
#define MACRO_SIZE 6 /*maximum lines in a macro*/
#define FILE_SIZE 260 /*maximum length of file name*/
#define MACHINE_CODE_SIZE 8192 /*the size of the RAM*/
#define ACTION_REGISTER_NUMBER 16 /*amount of the registers and the actions*/
#define START_IC 100 /*the first address of the machine code*/


typedef enum { FALSE, TRUE } boolean;
typedef enum { DATA, STRING, ENTRY, EXTERN, NONE } directiveType;


/*A struct for the actions. This struct contains the name fo the function, the opcode, the funct, and the amount of the operands this action can get. This is an arry of structs that initialized in the main file (it is a global array)*/
typedef struct {
   char name[5];
   int opcode;
   int funct;
   int operand_num;
}action;

/*An entry list - for the entry labels. This list contains the label, the base, the offset, the row that the label defined, and the "next" - pointer to the next node.*/
typedef struct entryList {
	char label[LABEL_SIZE];
	int base;
	int offset;
	int row;
  	struct entryList *next;
}entryList;

/*An extern list - for the extern labels. This list contains the label, the base, the offset, and the "next" - pointer to the next node.*/
typedef struct externList {
	char label[LABEL_SIZE];
	int base;
	int offset;
  	struct externList *next;
}externList;

/*A macro list - for the macros. This list contains the number of the lines in the macro, the name, the lines in the macro, and the "next" - pointer to the next node.*/
typedef struct macroList {
	int numOfLines;
	char name[LABEL_SIZE];
	char macroLines[MACRO_SIZE][MAX_SIZE];
  	struct macroList *next;
}macroList;

/*A symbol tabel list - for the labels. This list contains the label, the value(address), the base, the offset, the attributes, another attributes to "entry", and the "next" - pointer to the next node.*/
typedef struct symbolTabel {
	char symbol[LABEL_SIZE];
	int value;
	int base;
	int offset;
	char attributes[9];
	char attributes2[9];
  	struct symbolTabel *next;
}symbolTabel;


/*This is a struct for all the globals variabels. the struct contains:
1. The counters.
2. The arrays of the data and the code. In this array I will preforming the binary encoding and according this binary encoding, the ".ob" file will be created.
3. The head of the three lists in the program.*/
typedef struct globals{
	int IC;  /*counts the code (and all the information word).*/
	int DC;  /*counts the data.*/
	int DCF; /*counts the data value of the lines without labels.*/
	int L;   /*counts for each action the amount of information word to the current action.*/
	int indexInCodeA; /*signifies the current place in the code array in the first transition.*/
	int indexInData;  /*signifies the current place in the code array in the second transition.*/
	int indexInCodeB; /*signifies the current place in the data array in.*/
	int code[MACHINE_CODE_SIZE];
	int data[MACHINE_CODE_SIZE];
	entryList* ent_head;  /*head for entryList - the labels that defined as entry, with base and offset.*/
	externList* ext_head; /*head for externList - the labels that defined as entry, with base and offset.*/
	symbolTabel* st_head; /*head for symbolTabel - the labels with addres, base, offset, and attributes.*/
}globals;

/*functions*/
int first(char *fileName, char *line, int row, char *registers[]);
int firstTrans(char *fileName, FILE* f_am, char *registers[]);
int second(char *fileName, char *line, int row, char *registers[]);
int secondTrans(char *fileName, FILE* f_am, char *registers[]);
int checkLabelLine(char *fileName, char *line, char *word1, int row, int column, char *registers[]);
int checkEntryExtern(char *fileName, int row);
int checkActionLine(char *fileName, int trans, char *line, int opcode, int column, int row, char *registers[]);
int actions(char *word);
int isEntry(char *fileName, int column, char *line, int declare, char *registers[], int row);
int isExtern(char *fileName, int column, char *line, int declare, char *registers[], int row);
int isLabel(char *fileName, char *word, int declare, int row, char *registers[]);
int isData(char *fileName, char *newLine, int numOfRow, int column);
int isString(char *fileName, char *newLine, int numOfRow, int column);
int printerrors(char *fileName, int flag, int row, char *word);
int printfiles(char *file);
int checkOperand(char *fileName, char *operand, int opcode, int row, char *registers[]);
int checkOperands(char *fileName, char *operand1, char *operand2, int opcode, int row, char *registers[]);
int completeBaseOffset(char *fileName, char *operand1, char *operand2, int row, char *registers[]);
void addRE(int n, int base, int offset);
int operand0(char *fileName, char *actionLine, int column, int trans, int row);
int operand1(char *fileName, char *actionLine, int column, int trans, int opcode, int row, char *registers[]);
int operand2(char *fileName, char *actionLine, int column, int trans, int opcode, int row, char *registers[]);
boolean method1(char *fileName, char *operand, int method, int row);
boolean method2(char *fileName, char *operand, int method, int row, char *registers[]);
void method3(int j);
void method4(int method);
int addressing2(char *fileName, char *operand, int row, char *registers[]);
void fixContentInSymbolTable();
int whiteSpace(char *line, int i);
int baseCalc(int n);
int offsetCalc(int n);
char *nextWord(char *line, char *word, int column);
boolean emptyLine(char *line);
boolean longLine(char *line);
boolean macroSpan(char *fileName, FILE *f,FILE *f_am);
directiveType directive(char *word);
char *allocationWord(char *word, int size);
void freeSymbolTabelList();
void freeMacroList(macroList *head);
void freeEntryList();
void freeExternList();
macroList *addLine(int index, char* line, macroList* node);
macroList *addName(char *line, macroList* node);
macroList *addFirstToMacro(macroList* node, macroList* m_head, int lineCounter);
macroList *allocationMacroList(macroList* node);
entryList *addContentToEntryNode(entryList *node, char *word, int row);
entryList *addFirstToEntry(entryList *ent_node);
entryList *allocationEntryList(entryList* ent_node);
entryList *reverseEntryList();
externList *addContentToExternNode(externList* ext_node, char *label);
externList *addFirstToExtern(externList *ext_node);
externList *allocationExternList(externList* ext_node);
externList *reverseExternList();
symbolTabel *reverseSymbolTabelList();
symbolTabel *allocationSymbolTabelList();
symbolTabel *addFirstToSymbolTabel(symbolTabel* newp);
symbolTabel *addContentToSymbolTabelNode(symbolTabel *node,char *symbol,int value,int base,int offset,char *attributes);

extern globals *g;
extern action actArr[ACTION_REGISTER_NUMBER];
