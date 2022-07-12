#include "dataBase.h"
#include "define.h"

/*This function gets a line from the "data_string" function and analyze it.
The function gets 4 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
dataLine: the line to analyze.
row: the current row (using if the program needs to notify to the user about errors.)
column: the number of column in the current line.
variables:
i, j: index in char array.
comma: sign if the last char was ','.
finalNum: the final number in data (for each number).
counter: counts the anount of the number in the data.
mask: using for negetive numbers.
num: the number in the begining (after, the "atoi" function make the number - int).
operator: sign if there is '+' or '-' before the number.*/
int isData(char *fileName, char dataLine[], int row, int column) 
{
	boolean comma = FALSE;
	int i = column;
	int j = 0;
	int finalNum = 0;
	int counter = 0;
	char operator;
	char num[7];
	while(dataLine[i++] != '\n') { /*checks if all the chars are o.k for data line.*/
		if(!isdigit(dataLine[i]) && dataLine[i] != ',' && dataLine[i] != '+' && dataLine[i] != '-' && !isspace(dataLine[i])) {
			printerrors(fileName, ILLEGAL_DATA, row, "");
			return -2;
		}
	}
	while(dataLine[column] != '\n') {
		column = whiteSpace(dataLine, column); /*go to the next not white space char*/
		if(dataLine[column] == '\n') { /*end of line. last char was ',' - ERROR*/
			if(comma == TRUE) {
				printerrors(fileName, ILLEGAL_DATA, row, "");
				return -2;
			}
			return counter;
		}
		if(dataLine[column] == ',' && comma == TRUE) { /*the char is ',' - ERROR*/
			printerrors(fileName, ILLEGAL_DATA, row, "");
			return -2;
		}
		if(dataLine[column] == '-' || dataLine[column] == '+') { /*next char is '+'\'-', operator is char*/
			operator = dataLine[column];
			column++;
			if(!isdigit(dataLine[column])) {
				printerrors(fileName, ILLEGAL_DATA, row, "");
				return -2;
			}
		}
		while(isdigit(dataLine[column])) { /*keep the number in the array num*/
			num[j++] = dataLine[column++];
			comma = FALSE;
		}
		finalNum = atoi(num); /*finish the number*/
		if(operator == '-') {
			finalNum *= (-1);
			finalNum &= MAX_NUM;
		}
		g->data[g->indexInData] = A;
		g->data[g->indexInData] <<= MOVE_ARE;
		g->data[g->indexInData] += finalNum; /*enter the number to a new line in the data array.*/
		j = 0;
		finalNum = 0;
		operator = '\0';
		g->indexInData++;
		counter++;
		column = whiteSpace(dataLine, column); /*go to the next not white space char*/
		if(dataLine[column] == '\n') /*line finish - return counter*/
			return counter;
		if(dataLine[column] != ',') { /*the next char is not ',' - ERROR*/
			printerrors(fileName, ILLEGAL_DATA, row, "");
			return -2;
		}
		column++;
		if(dataLine[column] == '\n' || dataLine[column] == ',') { /*line finish after ',' - ERROR*/
			printerrors(fileName, ILLEGAL_DATA, row, "");
			return -2;
		}
	}
	return counter;
}

/*This function gets a line from the "data_string" function and analyze it.
The function gets 4 parameters:
fileName: the name of the current file (using if the program needs to notify to the user about errors.)
stringLine: the line to analyze.
row: the current row (using if the program needs to notify to the user about errors.)
column: the number of column in the current line.
variables:
i: index in char array.
length: the length of the string (with '\0').
string: the string inside the " ".*/
int isString(char *fileName, char stringLine[], int row, int column) 
{
	char string[MAX_SIZE] = {'\0'};
	int i;
	int j = 0;
	int length = 0;
	i = whiteSpace(stringLine, column); /*go to the next not white space char*/
	if(stringLine[i] != '"' || stringLine[strlen(stringLine) - 2] != '"') { /*the string doesn't start with " - ERROR*/
		printerrors(fileName, ILLEGAL_STRING, row, "");
		return -2;
	}
	while(i < (strlen(stringLine) - 1))
		string[j++] = stringLine[i++];
	string[j++] = stringLine[i++];
	string[j] = '\0';
	length = strlen(string) - 2;
	i = 1;
	while(i < (strlen(string) - 2)) { /*encode the chars in data array (each letter in new line).*/
		if(!isprint(string[i])) { /*if the ascii code isn't correcr code - ERROR*/
			printerrors(fileName, ILLEGAL_STRING, row, "");
			return -2;
		}
		g->data[g->indexInData] = A;
		g->data[g->indexInData] <<= MOVE_ARE;
		g->data[g->indexInData] += string[i];
		g->indexInData++;
		i++;
	}
	g->data[g->indexInData] = A;
	g->data[g->indexInData] <<= MOVE_ARE;
	g->indexInData++;
	return length;
}
