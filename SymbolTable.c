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
	#endif

	//Init status to NEW
	for(int i = 0; i < 769; i++)
	{
		(*(slots + i)).status = NEW;
	}

	//Init table
	table->slots = slots;
	table->size = 769;
	table->entries = 0;

	return table;
}

int ST_put(SymTab oSymTab, const char* key, const void* value)
{
	int hashIndex, notFound, count, result, spot;

	hashIndex = hash(key, oSymTab-> size);

	printf("HASHED: %i\n", hashIndex);

	//Init
	count = 0;
	notFound = 1;

	if(count == oSymTab->size)
	{
		//No slot found
		result = -1;
	}
	else
	{
		//While free slot not found and has room
		while(notFound == 1 && count < oSymTab->size)
		{
			//Calculate spot in circular array
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

		//Update slot values
		(*(oSymTab->slots + spot)).data = value;
		(*(oSymTab->slots + spot)).status = FULL;
		oSymTab->entries += 1;

		//Copy key to slot
		(*(oSymTab->slots + spot)).key = stringCopy(key);

		//Save string length to slot
		(*(oSymTab->slots + spot)).keyLength = stringLength((*(oSymTab->slots + spot)).key);

		printf("KEY SAVED AS: %s\n", (*(oSymTab->slots + spot)).key);

		result = 0;
	}

	return result;
}

int ST_contains(SymTab oSymTab, const char* key)
{
	int hashIndex, found, count, probing, spot;

	hashIndex = hash(key, oSymTab->size);

	//Init
	found = 0;
	count = 0;
	probing = 1;

	//While not found, haven't looked at all slots and still probing
	while(found == 0 && count < oSymTab->size && probing == 1)
	{
		//Calculate spot in circular array
		spot = ((hashIndex + count) % oSymTab->size);

		//Index is empty, stop searching
		if((*(oSymTab->slots + spot)).status == FULL && stringCompare(
			(*(oSymTab->slots + spot)).key, key))
		{
			found = 1;
		}
		else if((*(oSymTab->slots + spot)).status == USED)
		{
			//Keep probing
		}
		else
		{
			//Not found
			probing = 0;		
		}

		count++;		
	}

	return found;
}

void* ST_get(SymTab oSymTab, const char* key)
{
	int hashIndex, count, probing, spot;
	const void* rVal;

	//Init
	hashIndex = hash(key, oSymTab->size);
	count = 0;
	probing = 1;

	//While havent looked at all slots and still probing
	while(count < oSymTab->size && probing == 1)
	{
		//Calculate spot in circular array
		spot = ((hashIndex + count) % oSymTab->size);

		//Index is Full, search
		if((*(oSymTab->slots + spot)).status == FULL)
		{
			//Check if key matches
			if(stringCompare((*(oSymTab->slots + spot)).key, key))
			{
				probing = 0;
				rVal = (*(oSymTab->slots + spot)).data;
			}
			else
			{
				//Keep probing
			}
		}
		else if((*(oSymTab->slots + spot)).status == USED)
		{
			//Previously used, probe further
		}
		else
		{
			//Slot must be new, cannot be found
			probing = 0;
			rVal = NULL;		
		}

		count++;
	}

	return (void*)rVal;
}