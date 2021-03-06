/*Declarations for different states a slot can be in*/
#define NEW 0
#define FULL 1
#define USED 2

#define NULL ((void *)0)

/*Struct for storing each entry in the table*/
typedef struct
{
	int status;
	const void* data;
	char* key;
	int keyLength;
} Slot;

/*Struct that contains the array of entries, will be seen as "the table"*/
typedef struct
{
	int size;
	int entries;
	Slot* slots;
} symtab;

typedef symtab* SymTab;

/*Forward declaration*/
SymTab ST_new();
int ST_put(SymTab oSymTab, const char* key, const void* value);
int ST_contains(SymTab oSymTab, const char* key);
void* ST_get(SymTab oSymTab, const char* key);
int ST_remove(SymTab oSymTab, const char* key);
SymTab ST_resize(SymTab oSymTab);
void freeSlots(Slot* slots, int number);
void ST_free(SymTab oSymTab);