#include "SymbolTable.h"
#include "Utility.h"
#include <stdio.h>

int main()
{
	printf("START TEST\n");
	void* testData = NULL;
	SymTab table = NULL; 

	printf("\n+++++=TABLE=+++++\n\n");
	table = ST_new();

	printf("\n+++++==PUT==+++++\n\n");
	ST_put(table, "TEST KEY IS A TEST", testData);

	printf("\n+++++=CONTAINS=+++++\n\n");
	
	if(ST_contains(table, "TEST KEY IS A TEST") == 1)
	{
		printf("Contains: PASS\n");
	}
	else
	{
		printf("Contains: FAIL\n");
	}

	if(ST_contains(table, "This should fail") == 1)
	{
		printf("Does not contain: FAIL\n");
	}
	else
	{
		printf("Does not contain: PASS\n");
	}

	return 0;
}