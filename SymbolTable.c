#include "SymbolTable.h"
#include "Utility.h"

#ifdef sys
#include <sys/mman.h>
#include <unistd.h> //Double check
#endif

#ifdef lib
#include <stdlib.h>
#endif

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

	//Check if table should be resized
	ST_resize(oSymTab);

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

int ST_remove(SymTab oSymTab, const char* key)
{
	int hashIndex, found, count, probing, check, spot;

	//Init
	hashIndex = hash(key, oSymTab->size);
	found = 0;
	count = 0;
	probing = 1;
	check = 0;

	while(found == 0 && count < oSymTab->size && probing == 1)
	{
		spot = ((hashIndex + count) % oSymTab->size);

		//Slot is Full
		if((*(oSymTab->slots + spot)).status == FULL)
		{
			//Check if key matches
			if(stringCompare((*(oSymTab->slots + spot)).key, key))
			{
				found = 1;

				//Free memory used to store key
				#ifdef sys
				munmap((*(oSymTab->slots + spot)).key, 
				sizeof(char)*(stringLength((*(oSymTab->slots + spot)).key)));

				if(check != 0)
				{
					//write(2, "ST_remove: munmap failed");
				}
				#endif

				#ifdef lib
				free((*(oSymTab->slots + spot)).key);
				#endif

				//Decrement entries counter
				oSymTab->entries -= 1;

				//Change slot status to used
				(*(oSymTab->slots + spot)).status = USED;		
			}
		}
		//Previously used, probe further
		else if((*(oSymTab->slots + spot)).status == USED)
		{

		}
		//Slot must be new, cannot be found
		else
		{
			probing = 0;	
		}
	}	

	return found;	
}

//Grow or shrink the hash table, rebuilding it every time the size changes
//Rebuilding is expensive
SymTab ST_resize(SymTab oSymTab)
{
	int count, i;
	Slot* temp, *new;

	//Init
	count = 0;

	//If load factor greater than 0.5 grow
	if((oSymTab->entries / oSymTab->size) > 0.5 && oSymTab->size != *(sizes + 7))
	{
		//Work out what size to grow to
		while(oSymTab->size != *(sizes + count))
		{
			count++;
		}

		//Allocate larger array
		#ifdef sys
		new = (Slot*)mmap(NULL, sizeof(Slot) * (*(sizes + count + 1)), MMAP_PERM, MMAP_MODE, -1, 0);
		#endif

		#ifdef lib
		new = (Slot*)malloc(sizeof(Slot) * (*(sizes + count + 1)));
		#endif

		//Make new table
		temp = oSymTab->slots;

		//Assign new slots to table
		oSymTab->slots = new;

		//Update table data
		oSymTab->size = (*(sizes + count + 1));
		oSymTab->entries = 0;

		//Init slots as NEW
		for(i = 0; i < (*(sizes + count + 1)); i++)
		{
			(*(oSymTab->slots + i)).status = NEW;
		}

		//Move entries from old to new table
		for(i = 0; i < (*(sizes + count)); i++)
		{
			if((*(oSymTab->slots + i)).status == FULL)
			{
				ST_put(oSymTab, (*(temp + i)).key, (*(temp + i)).data);
			}
		}

		//Free slots from old table
		freeSlots(temp, oSymTab->size);
	}

	return oSymTab;
}

//Free values stored in slots and the slots
void freeSlots(Slot* slots, int number)
{
	int i, check;

	check = 0;

	for(i = 0; i < number; i++)
	{
		//If full, free key
		if((*(slots + i)).status == FULL)
		{
			#ifdef sys
			munmap((*(slots + i)).key, sizeof(char) * ((*(slots + i)).keyLength));
			munmap(slots, sizeof(Slot) * number);

			//Error checking
			if(check != 0)
			{
				write(2, "freeSlots: munmap failed", 25);
			}
			#endif

			#ifdef lib
			free((*(slots + i)).key);
			free(slots);
			#endif
		}		
	}
}

//Wrapper for freeSlots so you can pass a SymTab struct instead
void ST_free(SymTab oSymTab)
{
	freeSlots(temp, oSymTab->size);
}