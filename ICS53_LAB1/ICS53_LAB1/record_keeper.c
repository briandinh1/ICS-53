#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *inFile, *outFile;
char records[100][80]; // at most 100 records with 80 characters per line
int numElements = 0; // keep track of current number of records

void read(char fname[])
{
	inFile = fopen(fname, "r");

	if (!inFile)
	{
		perror("Cannot open input file");
		return;
	}

	numElements = 0;
	char line[80];

	while (fgets(line, sizeof(line), inFile))
		strcpy(records[numElements++], line);

	fclose(inFile);

	printf("Input file has been read into memory.");
}

void write(char fname[])
{
	if (numElements == 0)
	{
		printf("There is no record currently loaded into memory.");
		return;
	}

	outFile = fopen(fname, "w");

	if (!outFile)
	{
		perror("Cannot open output file");
		return;
	}

	for (int i = 0; i < numElements; ++i)
		fprintf(outFile, "%s", records[i]);

	fclose(outFile);

	printf("Records have been written to output file.");
}

void print(void)
{
	if (!inFile)
	{
		printf("There is no record currently loaded into memory.");
		return;
	}

	if (numElements == 0)
	{
		printf("There are no records to print.");
		return;
	}

	for (int i = 0; i < numElements; ++i)
		printf("%d\t%s", i + 1, records[i]);

	printf("\n\nFile has been printed.");
}

void delete_file(char lineIndex[])
{
	if (numElements == 0)
	{
		printf("There is no record currently loaded into memory.");
		return;
	}

	int index = strtol(lineIndex, 0, 10);

	if (index < 1 || index > numElements)
	{
		printf("Invalid input.");
		return;
	}

	for (int i = index - 1; i < numElements; ++i)
		strcpy(records[i], records[i + 1]);

	--numElements;

	printf("Record %d has been deleted.", index);
}


void runProgram(void)
{
	while (1)
	{
		printf("Enter a command (read, write, print, delete, quit): ");
		char line[80];
		gets(line);
		if (strcmp(line, "") == 0) { continue; }
		printf("\n");

		// get first token (command)
		char *command = strtok(line, " \t");

		if (strcmp(command, "quit") == 0) return;
		if (strcmp(command, "print") == 0)
		{
			print();
			printf("\n");
			continue;
		}

		//get next token (argument), if any
		char *argument = strtok(NULL, " \t");
		if (strcmp(command, "read") == 0) read(argument);
		else if (strcmp(command, "write") == 0) write(argument);
		else if (strcmp(command, "delete") == 0) delete_file(argument);
		else printf("That is not one of the choices. Try again.");

		printf("\n");
	}
}

int main(void)
{

	return 0;
}