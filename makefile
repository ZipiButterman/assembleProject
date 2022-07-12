assembler: main.o code.o print.o transition1.o transition2.o auxiliaryFunctions.o listFunctions.o data.o macro.o
	gcc -ansi -pedantic -Wall -g main.o code.o print.o transition1.o transition2.o auxiliaryFunctions.o listFunctions.o data.o macro.o -o assembler
main.o: main.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall main.c -o main.o
code.o: code.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall code.c -o code.o
print.o: print.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall print.c -o print.o
transition1.o: transition1.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall transition1.c -o transition1.o
transition2.o: transition2.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall transition2.c -o transition2.o
auxiliaryFunctions.o: auxiliaryFunctions.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall auxiliaryFunctions.c -o auxiliaryFunctions.o
listFunctions.o: listFunctions.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall listFunctions.c -o listFunctions.o
data.o: data.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall data.c -o data.o
macro.o: macro.c dataBase.h define.h
	gcc -c -ansi -pedantic -Wall macro.c -o macro.o
