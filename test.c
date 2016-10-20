#include "SymbolTable.h"
#include "Utility.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int exit = 0;
	int option = 0;
	char name[20];
	int* data;
	int count = 0;
	int i = 0;

	SymTab book = ST_new();

	while(exit == 0)
	{
		printf("#####ENTER OPTION#####\n");
		printf("1) Add Entry\n");
		printf("2) Find Entry\n");
		printf("3) Remove Entry\n");
		printf("4) Fill to capacity\n");
		printf("5) Free fill to capacity\n");
		printf("6) Exit\n\n");
	
		printf("Option: ");

		scanf("%d", &option);

		if(option == 1)
		{
			//Add
			//Name is used as the key
			if(count < 98317)
			{
				printf("\nEnter Name: ");
				scanf("%s", name);

				data = (int*)malloc(sizeof(int));
				printf("Enter phone number: ");
				scanf("%d", data);

				printf("\n");

				ST_put(book, name, (void*)data);

				free(data);

				count++;
			}
			else
			{
				printf("Can't add\n");
			}
		}
		else if(option == 2)
		{
			//Get
			//Name is used as the key
			printf("\nEnter Name: ");
			scanf("%s", name);
			printf("\n");

			if(ST_contains(book, name) == 1)
			{
				printf("Name: %s\n", name);
				printf("Ph: %i\n\n", (*(int*)ST_get(book, name)));
			}
			else
			{
				printf("Person not found\n\n");
			}
		}
		else if(option == 3)
		{
			//Name is used as the key
			printf("\nEnter Name: ");
			scanf("%s", name);
			printf("\n");

			//Remove
			if(ST_contains(book, name) == 1)
			{	
				free(ST_get(book, name));
				ST_remove(book, name);
				count--;
			}
			else
			{
				printf("Person not found");
			}
		}
		else if(option == 4)
		{
			for(i = count; i < 98317; i++)
			{
				sprintf(name, "%d", i);
				data = (int*)malloc(sizeof(int));
				*data = i;
				ST_put(book, name, data);
				count++;
			}
		}
		else if(option == 5)
		{
			for(i = 0; i < 98317; i++)
			{
				sprintf(name, "%d", i);
				*data = i;
				free(ST_get(book, name));
				count++;
			}
		}
		else if(option == 6)
		{
			exit = 1;
		}
		else
		{
			printf("\nInvalid option\n\n");
		}
	}

	ST_free(book);

	return 0;
}