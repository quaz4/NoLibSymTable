#include "Utility.h"
#include <stdio.h> //DELETE ME

#define MMAP_PERM PROT_READ|PROT_WRITE
#define MMAP_MODE MAP_PRIVATE|MAP_ANONYMOUS
#include <sys/mman.h>

//Hashes a string, returning a number
int hash(const char* inKey, int tableSize)
{
	int hashIndex;
	int i;

	hashIndex = 0;

	for(i = 0; i < stringLength(inKey); i++)
	{
		hashIndex = (31 * hashIndex) + (int)(*(inKey + i));
	}

	if(hashIndex < 0)
	{
		hashIndex = hashIndex * -1;
	}

	return hashIndex % tableSize;
}

/*Counts the number of characters in a string, not including \0*/
int stringLength(const char* inString)
{
	int i;

	i = 0;

	while( (*(inString + i)) != '\0')
	{
		i++;
	}

	return i;
}

//Tests if two strings are the same
int stringCompare(const char* str1, const char* str2)
{
	int c;
	int equals;

	equals = 1;
	c = 0;
	
	//Test if legnth is the same
	if(stringLength(str1) != stringLength(str2))
	{
		//Not the same length
		equals = 0;
	}
	else
	{
		//Same length
		//Loop through string comparing each char, exits loop on fail
		while((*(str1 + c)) != '\0' && equals == 1)
		{
			if((*(str1 + c)) == (*(str2 + c)))
			{
				c++;
			}
			else
			{
				equals = 0;
			}
		}
	}

	return equals;
}

/*Takes a string and copies it, returning the copy as a pointer*/
char* stringCopy(const char* original)
{
	int c = 0;

	char* target = NULL;

	//#ifdef sym
	//target = (char*)mmap(NULL, sizeof(char) * (stringLength(original)+1), MMAP_PERM, MMAP_MODE, -1, 0);
	//#endif

	target = (char*)mmap(NULL, sizeof(char) * (50), MMAP_PERM, MMAP_MODE, -1, 0);

	while((*(original + c)) != '\0')
	{
		printf("ORIGINAL(%i): %c\n", c, (*(original + c)));
		(*(target + c)) = (*(original + c));
		c++;
	}

	return target;
}