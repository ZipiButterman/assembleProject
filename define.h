#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define A 4 /*the value of A in the first part*/
#define R 2 /*the value of R in the first part*/
#define E 1 /*the value of E in the first part*/
#define FUNCT 4 /*the number of bits of FUNCT part*/
#define SOURCE_REGISTER 4 /*the number of bits of SOURCE REGISTER part*/
#define ADDRESSING_SOURCE 2 /*the number of bits of ADDRESSING SOURCE part*/
#define TARGET_REGISTER 4 /*the number of bits of TARGET REGISTER part*/
#define ADDRESSING_TARGET 2 /*the number of bits of ADDRESSING TARGET part*/
#define MOVE_ARE 16 /*the number of bits to move, after the encoding of ARE part*/
/*ERRORS - When the program finds an error - then it prints to the standart output the file of the error, the line of the error and a little description and step to the next line. (of-corse, in this case won't create the output files)*/
#define WARNING_LABEL 400 /*if there is a label before .entry or .extern directive.*/
#define EMPTY_FILE 401 /*if the current file is an empty file.*/
#define ILLEGAL_SOURCE 402 /*if source operand illegal to this opcode.*/
#define ERROR_IN_FILE 403 /*all the errors in a specific file.*/
#define COMMA 404 /*if there is a comma not in the place (like one after the other and more).*/
#define NOT_COMMA 405 /*if there are 2 operands without comma bitween them.*/
#define MANY_OP 406 /*if there are too many operands for one action.*/
#define LABEL_NOT_FOUND 407 /*if there is a label that not in the symboltabel.*/
#define ILLEGAL_REGIST 408 /*if there is an illegal register in the index.*/
#define ILLEGAL_LINE 409 /*if there is a line that the first word is not legal word.*/
#define NO_FILE 410 /*if there are no filesin the command line.*/
#define WORNG_FILE 411 /*if the name of the file is worng.*/
#define ILLEGAL_OPERAND 412 /*if the operand is worng.*/
#define ILLEGAL_DATA 413 /*if the data line is worng.*/
#define ILLEGAL_STRING 414 /*if the string line is worng.*/
#define ILLEGAL_LABEL 415 /*if the name of the label is illegal (like 1MAIN).*/
#define LABEL_TWICE 416 /*if there is a label that already defined.*/
#define NO_MEMMORY 417 /*if there is no enough memmory.*/
#define NO_OPERAND2 418 /*if there is just one operand (instead of 2).*/
#define NO_OPERAND1 419 /*if there is no operand (instead of 1).*/
#define LONG_LINE 420 /*if there is too long line.*/
#define ENTRY_EXTERN 421 /*if there is label that defined as both entry and extern.*/
#define EXTERN_DEFINE 422 /*if there is label that declared as extern and also as regular.*/
#define JUST_ENTRY 423 /*if there is label declared as entry, and in practice didn't declared.*/
#define ILLEGAL_COMMAND 425 /*an unknown command (foreach first word in line without ":" in end or "." in the begining)*/
#define LABEL_SPACE 426 /*after label must be a space.*/
#define WARNING_EXTERN 427 /*if an extern label defined twice.*/
#define WARNING_ENTRY 428 /*if an entry label defined twice.*/
#define NUMBER_REGIST 429 /*the number of register inside "[]" isn't 10-15.*/
#define MUST_REGIST 430 /*"[]" is empty.*/
#define FIRST_LETTER 431 /*first char of a label isn't a letter.*/
#define ALPHANUMERIC_LABEL 432 /*chars in a label are not letters or numbers.*/
#define RESERVED_WORD 433 /*label is a reserved word.*/
#define LONG_LABEL 434 /*label is too long.*/
#define ILLEGAL_NUMBER 435 /*number isn't in the range (MIN_NUM-MAX_NUM).*/
#define ILLEGAL_TARGET 436 /*if target operand illegal to this opcode.*/
#define INDEX_OPERAND 437 /*if target operand illegal to this opcode.*/
#define ILLEGAL_DIRECTIVE 438 /*if target operand illegal to this opcode.*/


