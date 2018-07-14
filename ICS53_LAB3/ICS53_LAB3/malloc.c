// Dinh, Brian: 34536266
// Catanghal, Margarette: 90338978

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAPSIZE 400
#define UNALLOCATED 0
#define ALLOCATED 1
#define BLOCK 2
#define PAD ALLOCATED+BLOCK

int blockNum = 1;
unsigned char * heap = NULL;

void allocate(int numBytes)
{
	int i = 0;

	// max payload size
	if (numBytes > 255 - PAD) return;

	while (i < HEAPSIZE)
	{
		if (i + numBytes + PAD >= HEAPSIZE) break;

		// if there is no existing free block for the payload
		// but there is still enough space elsewhere on the heap
		if (heap[i] == '\0')
		{
			heap[i] = numBytes + PAD;
			heap[i + ALLOCATED] = ALLOCATED;
			heap[i + BLOCK] = blockNum;
			printf("%i\n", blockNum++);
			break;
		}
		// if there is an existing free block that is 
		// large enough for the new payload
		else if (heap[i] >= numBytes + PAD
			&& heap[i + ALLOCATED] == UNALLOCATED)
		{
			int newOffset = numBytes + PAD;
			int remainder = heap[i] - newOffset;
			heap[i] = newOffset;
			heap[i + ALLOCATED] = ALLOCATED;
			heap[i + BLOCK] = blockNum;
			if (remainder != 0)
			{
				heap[i + newOffset] = remainder;
				heap[i + newOffset + ALLOCATED] = UNALLOCATED;
			}
			printf("%i\n", blockNum++);
			break;
		}

		i += heap[i];
	}
}

void freeBlock(int blockNum)
{
	int i = 0;

	while (i < HEAPSIZE)
	{
		if (heap[i] == '\0') break;

		if (heap[i + BLOCK] == blockNum)
		{
			// set block data to unallocated
			// payload data remains but cannot be accessed anymore
			heap[i + ALLOCATED] = UNALLOCATED;
			break;
		}

		i += heap[i];
	}
}

void blockList(void)
{
	printf("%-*s", 7, "Size");
	printf("%-*s", 12, "Allocated");
	printf("%-*s", 12, "Start");
	printf("%-*s", 12, "End");
	printf("\n");

	int i = 0;

	while (i < HEAPSIZE)
	{
		if (heap[i] == '\0') break;

		// print block size (header + payload)
		printf("%-*i", 7, heap[i] - 1);

		// print block allocated or unallocated
		printf("%-*s", 12, (heap[i + ALLOCATED] == ALLOCATED) ? "Yes" : "No");

		// print block start address in hex
		printf("%s%-*x", "0x", 10, &(heap[i]));

		// print block end address in hex
		printf("%s%-*x", "0x", 10, &(heap[i + heap[i] - 1]));

		printf("\n");
		i += heap[i];
	}
}

void writeHeap(int blockNum, char c, int num)
{
	int i = 0;

	while (i < HEAPSIZE)
	{
		if (heap[i] == '\0') break;

		if (heap[i + BLOCK] == blockNum
			&& heap[i + ALLOCATED] == ALLOCATED)
		{
			// possible to overwrite into next block (done on purpose)
			for (int j = 0; j < num; ++j)
				heap[i + PAD + j] = c;

			break;
		}

		i += heap[i];
	}
}

void printHeap(int blockNum, int num)
{
	int i = 0;

	while (i < HEAPSIZE)
	{
		if (heap[i] == '\0') break;

		if (heap[i + BLOCK] == blockNum
			&& heap[i + ALLOCATED] == ALLOCATED)
		{
			for (int j = 0; j < num; ++j)
			{
				int payload = i + PAD + j;

				// print ascii character values of the block's payload
				// print garbage if user gives a value > payload size
				(heap[payload] >= 0 && heap[payload] <= 255 && heap[payload] != '\0')
					? printf("%c", heap[payload])
					: printf("%i", (int)&(heap[payload]));
			}

			printf("\n");
			break;
		}

		i += heap[i];
	}
}

void runProgram(void)
{
	while (1)
	{
		printf("> ");
		char line[80];
		gets(line);
		if (strcmp(line, "") == 0) { continue; }

		// get first token (command)
		char *command = strtok(line, " \t");

		if (strcmp(command, "quit") == 0) return;
		if (strcmp(command, "blocklist") == 0)
		{
			blockList();
			continue;
		}

		//get all next arguments, if any
		char *argument = strtok(NULL, " \t");

		if (strcmp(command, "allocate") == 0) allocate(atoi(argument));
		else if (strcmp(command, "free") == 0) freeBlock(atoi(argument));
		else if (strcmp(command, "writeheap") == 0)
		{
			int blockNum_ = atoi(argument);
			char c = *(strtok(NULL, " \t"));
			int num = atoi(strtok(NULL, " \t"));
			writeHeap(blockNum_, c, num);
		}
		else if (strcmp(command, "printheap") == 0)
		{
			int blockNum_ = atoi(argument);
			int num = atoi(strtok(NULL, " \t"));
			printHeap(blockNum_, num);
		}
	}
}

int main(void)
{
	heap = malloc(HEAPSIZE);
	for (int i = 0; i < HEAPSIZE; ++i) heap[i] = '\0';

	runProgram();

	free(heap);
	return 0;
}