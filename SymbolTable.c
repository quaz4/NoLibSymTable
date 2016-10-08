#include "SymbolTable.h"
#include "Utility.h"

#ifdef sys
#include <sys/mman.h>
#endif

#ifdef lib
#include <stdlib.h>
#endif

#include <stdio.h> //DELETE ME

#define MMAP_PERM PROT_READ|PROT_WRITE
#define MMAP_MODE MAP_PRIVATE|MAP_ANONYMOUS

const int sizes[8] = {769, 1543, 3079, 6151, 12289, 24593, 49157, 98317};

SymTab ST_new()
{
	SymTab table;
	Slot* slots;

	//Allocate memory for table and slots
	#ifdef sys 
	slots = (Slot*)mmap(NULL, sizeof(Slot)*769, MMAP_PERM, MMAP_MODE, -1, 0);
	table = (SymTab)mmap(NULL, sizeof(symtab), MMAP_PERM, MMAP_MODE, -1, 0);
	/*slots = (Slot*)mmap(NULL, sizeof(Slot)*769, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	table = (SymTab)mmap(NULL, sizeof(symtab), PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0); */
	#endif

	//Init status to NEW
	for(int i = 0; i < 769; i++)
	{
		(*(slots + i)).status = NEW;
		//printf("STATUS(%i): %i\n", i, (*(slots + i)).status);
	}

	table->slots = slots;
	table->size = 769;
	table->entries = 0;

	return table;
}

int ST_put(SymTab oSymTab, const char* key, const void* value)
{
	int hashIndex;
	int notFound;
	int count;
	int result;
	int spot;

	hashIndex = hash(key, oSymTab-> size);

	printf("HASHED: %i\n", hashIndex);

	count = 0;
	notFound = 1;

	if(count == oSymTab->size)
	{
		//No slot found
		result = -1;
	}
	else
	{
		while(notFound == 1 && count < oSymTab->size)
		{
			spot = ((hashIndex + count) % oSymTab->size);

			if((*(oSymTab->slots + spot)).status == NEW)
			{
				//Slot is empty, stop searching
				notFound = 0;
			}
			else if((*(oSymTab->slots + spot)).status == USED)
			{
				//Slot is empty, stop searching
				notFound = 0;
			}
			else
			{
				//Slot is full, loop again, checking the next slot along
				count++;
			}
		}

		(*(oSymTab->slots + spot)).data = value;
		(*(oSymTab->slots + spot)).status = FULL;
		oSymTab->entries += 1;

		(*(oSymTab->slots + spot)).key = stringCopy(key);

		(*(oSymTab->slots + spot)).keyLength = stringLength((*(oSymTab->slots + spot)).key);

		printf("KEY SAVED AS: %s\n", (*(oSymTab->slots + spot)).key);

		result = 0;
	}

	return result;
}

int ST_contains(SymTab oSymTab, const char* key)
{
	return 0;
}