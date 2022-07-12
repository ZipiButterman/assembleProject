#include "dataBase.h"
#include "define.h"

/*allocations functions*/
entryList *allocationEntryList(entryList* ent_node) 
{
	 ent_node = (entryList*)malloc(sizeof(entryList));
	 if(ent_node == NULL) {
	 	printf("Failed to allocate space to list. exiting.\n");
	 	exit(1);
	 }
	 return ent_node;
}

externList *allocationExternList(externList* ext_node) 
{
	 ext_node = (externList*)malloc(sizeof(externList));
	 if(ext_node == NULL) {
	 	printf("Failed to allocate space to list. exiting.\n");
	 	exit(1);
	 }
	 return ext_node;
}


symbolTabel *allocationSymbolTabelList(symbolTabel* node) 
{
	 node = (symbolTabel*)malloc(sizeof(symbolTabel));
	 if(node == NULL) {
	 	printf("Failed to allocate space to list. exiting.\n");
	 	exit(1);
	 }
	 return node;
}

macroList *allocationMacroList(macroList *node) 
{
	 node = (macroList*)malloc(sizeof(macroList));
	 if(node == NULL) {
	 	printf("Failed to allocate space to list. exiting.\n");
	 	exit(1);
	 }
	 return node;
}

/*add content functions*/
symbolTabel *addContentToSymbolTabelNode(symbolTabel *node, char *symbol, int value, int base, int offset, char *attributes) 
{
	strtok(symbol, ":");
	strcpy((node->symbol), symbol);
	node->value = value;
	node->base = base;
	node->offset = offset;
	strcpy((node->attributes), attributes);
	node->next = g->st_head;
	return node;
}

externList *addContentToExternNode(externList *ext, char label[]) 
{
	strcpy(ext->label, label);
	ext->base = g->indexInCodeB + START_IC;
	ext->offset = g->indexInCodeB + START_IC + 1;
	ext->next = g->ext_head;
	return ext;
}
entryList *addContentToEntryNode(entryList *node, char *word, int row) 
{
	strcpy(node->label, word);
	node->row = row;
	node->next = g->ent_head;
	return node;
}

macroList *addLine(int index, char* line, macroList* node) 
{
    strcpy((node->macroLines[index]), line);
    return node;
}

macroList *addName(char *name, macroList* node) 
{
	strcpy((node->name), name);
	return node;	
}




macroList *addFirstToMacro(macroList* node, macroList* m_head, int lineCounter) 
{
	node->numOfLines = lineCounter;
	(node)->next = m_head;
	return node;
}

/*free list functions*/
void freeSymbolTabelList() 
{
	symbolTabel *node;
	while(g->st_head != NULL) {
		node = g->st_head;
		g->st_head = g->st_head->next;
		free(node);
	}
}

void freeMacroList(macroList *head) 
{
	macroList *node;
	while(head != NULL) {
		node = head;
		head = head->next;
		free(node);
	}
}

void freeEntryList() 
{
	entryList *node;
	while(g->ent_head != NULL) {
		node = g->ent_head;
		g->ent_head = g->ent_head->next;
		free(node);
	}
}

void freeExternList() 
{
	externList *node;
	while(g->ext_head != NULL) {
		node = g->ext_head;
		g->ext_head = g->ext_head->next;
		free(node);
	}
}

/*reverse list functions*/
symbolTabel *reverseSymbolTabelList() 
{
	symbolTabel *node = NULL;
	while (g->st_head != NULL) {
		symbolTabel *newNode = g->st_head;
		g->st_head = g->st_head->next;
		newNode->next = node;
		node = newNode;
	}
	return node;
}

entryList *reverseEntryList() {
	entryList *node = NULL;
	while (g->ent_head != NULL) {
		entryList *newNode = g->ent_head;
		g->ent_head = g->ent_head->next;
		newNode->next = node;
		node = newNode;
	}
	return node;
}

externList *reverseExternList() 
{
	externList *node = NULL;
	while (g->ext_head != NULL) {
		externList *newNode = g->ext_head;
		g->ext_head = g->ext_head->next;
		newNode->next = node;
		node = newNode;
	}
	return node;
}

/*This function updates the base and offset of the labels that their attributes is "data".
parameters:
oldValue: foreach label ("data"), there is in the value - the number of the lines that this data (or string) need. and also, foreach label ("data") there is the number of "data" or "string" lines that are in the code without label. The oldValue gets foreach label the value and add it to ICF after updates the base and offset in the label. Now the value, base, and offset of the labels are updates.
ICF: gets the value of IC, and this is the value to make on it the calculations.*/
void fixContentInSymbolTable() 
{
	int oldValue = 0, ICF = g->IC;
	symbolTabel* node;
	g->st_head = reverseSymbolTabelList(); /*the list is reverse*/
	node = g->st_head;
	while(node != NULL) {
		if(!strcmp(node->attributes, "data")) {
			oldValue = node->value;
			node->value = ICF;
			node->base = baseCalc(ICF);
			node->offset = offsetCalc(ICF);
			ICF += oldValue;
		}
		node = node->next;
	}
	
}

/*This function checks if there is a label that defined as both "extern" and "entry", if so - ERROR
The function gets 2 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
row: the current row (using if the program needs to notify to the user about errors.)*/
int checkEntryExtern(char *fileName, int row) 
{
	symbolTabel* node;
	node = g->st_head;
	while(node != NULL) {
		if(!strcmp(node->attributes, "external") && !strcmp(node->attributes2, "entry")) {
			printerrors(fileName, ENTRY_EXTERN, row, node->symbol);
			return 1;
		}
		node = node->next;
	}
	return 0;
}
