#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable: 4996)

struct todoList {
	char item[64];
	char date[32];
	char status[32];
	struct todoList* next;
} *list = NULL;

void flushStdIn();
void executeAction(char c);
void enterItem();
void addNode(char* tasks, char* dates);
void displayAll();
void complete();
void incomplete();
void delete();
void save(char* fileName);
void load(char* fileName);

int main()
{
	load("todoList.txt");
	char choice = 'i';
	do {
		printf("To-do list options\n");
		printf("\t a: Create task\n");
		printf("\t b: View incomplete tasks\n");
		printf("\t c: View all tasks\n");
		printf("\t d: Delete task\n");
		printf("\t e: Mark task as complete\n");
		printf("\t q: Quit\n");
		choice = getchar();
		flushStdIn();
		executeAction(choice);
	} while (choice != 'q');
	save("todoList.txt");
	return 0;
}

void flushStdIn()
{
	char c;
	do c = getchar();
	while (c != '\n' && c != EOF);
}

void executeAction(char c)
{

	switch (c)
	{
	case 'a':
		enterItem();
		break;
	case 'b':
		incomplete();
		break;
	case 'c':
		displayAll();
		break;
	case 'd':
		delete();
		break;
	case 'e':
		complete();
		break;
	case 'q':
		break;
	default: printf("\n%c is invalid input!\n", c);
	}
}

void enterItem()
{
	char date[32];
	char task[64];
	struct todoList* printList = NULL;

	printf("\nEnter the task: "); scanf("%s", task);
	printf("Please enter the due date (MM/DD): "); scanf("%s", date);
	addNode(task, date);

	printf("\nTask added\n\n");
	flushStdIn();
}

void addNode(char* tasks, char* dates)
{
	int count = 0;
	struct todoList* p;
	struct todoList* tempList = NULL;
	p = (struct todoList *)malloc(sizeof(struct todoList));
	strcpy(p->item, tasks);
	strcpy(p->status, "Status: Incomplete");
	strcpy(p->date, dates);
	p->next = NULL;
	if (list == NULL)		
		list = p;
	else if (strcmp(p->date, list->date) < 0)
	{
		p->next = list;
		list = p;
	}
	else
	{
		tempList = list;
		while (count == 0)
		{
			if ((strcmp(p->date, tempList->date) >= 0) && (tempList->next == NULL))
			{
				tempList->next = p;
				count++;
			}
			else if ((strcmp(p->date, tempList->date) > 0) && (strcmp(p->date, tempList->next->date) <= 0))
			{
				p->next = tempList->next;
				tempList->next = p;
				count++;
			}
			else
				tempList = tempList->next;
		}
	}
}

void displayAll()
{
	int count = 0;
	struct todoList* printList = list;
	printf("\nHere are your tasks: \n\n");
	while (printList != NULL)
	{
		printf(" %s ", printList->item);
		printf("%s ", printList->date);
		printf("%s\n\n", printList->status);
		printList = printList->next;
		count++;
	}
	if (count == 0)
		printf("You have no tasks");
}

void complete()
{
	struct todoList* tempList = list;
	char task[64];
	printf("Please enter the name of the completed task: "); scanf("%s", task); 
	while (strcmp(tempList->item, task) != 0)
	{
		if (tempList->next == NULL)
			printf("\nTask not found\n");
		else
			tempList = tempList->next;
	}
	if (strcmp(tempList->item, task) == 0)
	{
		strcpy(tempList->status, "Status: Completed");
	}
	printf("\nTask marked as complete\n\n");
	flushStdIn();
}

void incomplete()
{
	int count = 0;
	struct todoList* printList = list;
	printf("\nHere are your incomplete tasks: \n");
	while (printList != NULL)
	{
		if (strcmp(printList->status, "Status: Incomplete") == 0)
		{
			printf("\n%s; ", printList->item);
			printf("%s; ", printList->date);
			printf("%s\n\n", printList->status);
			count++;
		}
		printList = printList->next;
	}
	if (count == 0)
		printf("\nYou have no incomplete tasks\n\n");
}

void delete()
{
	struct todoList* tempList = list;
	char task[64];
	printf("Please enter the task you want to delete: "); scanf("%s", task);
	if (list == NULL)
	{
		printf("\nList is empty\n");
		return;
	}
	if (strcmp(list->item, task) == 0)
	{
		list = list->next;
		printf("\nTask deleted\n\n");
		flushStdIn();
	}
	else
	{
		while (strcmp(tempList->next->item, task) != 0)
		{
			if (tempList->next->next == NULL)
			{
				printf("\nTask not found\n\n");
				break;
			}
			else
				tempList = tempList->next;
		}
	}
	if (strcmp(tempList->next->item, task) == 0)
	{
		if (tempList->next->next == NULL)
			tempList->next = NULL;
		else
			tempList->next = tempList->next->next;
		printf("\nTask deleted\n\n");
		flushStdIn();
	}
}

void save(char* fileName)
{
	FILE* file;
	struct todoList* tempList = list;
	file = fopen(fileName, "w");

	while (tempList != NULL)
	{
		fwrite(tempList->item, strlen(tempList->item), 1, file);
		fprintf(file, "\n");
		fwrite(tempList->date, strlen(tempList->date), 1, file);
		fprintf(file, "\n");
		fwrite(tempList->status, strlen(tempList->status), 1, file);
		fprintf(file, "\n");
		tempList = tempList->next;
	}
	fclose(file);
}

void load(char* fileName)
{
	int ret;
	FILE* file;
	file = fopen(fileName, "r");

	if (file == NULL)
		return;
	else
	{
		struct todoList* p;
		p = (struct todoList *)malloc(sizeof(struct todoList));
		char task[64];
		char date[32];
		char status[32];
		fgets(task, 64, file);
		fgets(date, 32, file);
		fgets(status, 32, file);
		strcpy(p->item, task);
		strcpy(p->date, date);
		strcpy(p->status, status);
		p->next = NULL;
		list = p;
		struct todoList* tempList = list;
		while (fgets(task, 64, file) != NULL)
		{
			struct todoList* p;
			p = (struct todoList *)malloc(sizeof(struct todoList));
			fgets(date, 32, file);
			fgets(status, 32, file);
			strcpy(p->item, task);
			strcpy(p->date, date);
			strcpy(p->status, status);
			p->next = NULL;
			tempList->next = p;
			tempList = tempList->next;
		}
		ret = remove("todoList.txt");
	}
}