/*Forward declarations*/
char* stringCopy(const char* original);
int stringLength(const char* inString);
int hash(const char* inKey, int tableSize);
int stringCompare(const char* str1, const char* str2);

#define MMAP_PERM PROT_READ|PROT_WRITE
#define MMAP_MODE MAP_PRIVATE|MAP_ANONYMOUS

#define NULL ((void *)0)