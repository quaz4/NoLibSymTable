#include "SymbolTable.h"
#include "Utility.h"
#include <stdio.h>

int main()
{
	void* testData = "PASS";
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

	printf("\n+++++==GET==+++++\n\n");

	printf("Get test: %s\n", (char*)ST_get(table, "TEST KEY IS A TEST"));

	if(ST_get(table, "This should fail") == NULL)
	{
		printf("Get does not contain test: PASS\n");
	}
	else
	{
		printf("Get does not contain test: FAIL\n");
	}

	printf("\n+++++=REMOVE=+++++\n\n");	
	if(ST_remove(table, "TEST KEY IS A TEST") == 1)
	{
		printf("REMOVE: PASS\n");
	}
	else
	{
		printf("REMOVE: FAIL\n");
	}

	ST_free(table);

	return 0;
}