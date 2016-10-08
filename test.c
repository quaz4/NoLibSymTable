#include "SymbolTable.h"
#include "Utility.h"
#include <stdio.h>

int main()
{
	printf("START TEST\n");
	void* testData = NULL;
	SymTab table = NULL; 

	printf("+++++=TABLE=+++++\n");
	table = ST_new();

	printf("+++++==PUT==+++++\n");
	ST_put(table, "TEST KEY IS A TEST", testData);

	return 0;
}