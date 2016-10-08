Test: SymbolTable.c Utility.c test.c
	gcc -o Test -Wall -pedantic -std=gnu99 SymbolTable.c Utility.c test.c
sys: SymbolTable.c Utility.c test.c
	gcc -o Test -Wall -pedantic -std=gnu99 -Dsys SymbolTable.c Utility.c test.c