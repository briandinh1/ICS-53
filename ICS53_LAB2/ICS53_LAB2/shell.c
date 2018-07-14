#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#pragma warning(disable: 4996)

char line[BUFSIZ];

char *lsh_read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate a buffer for us
	getline(&line, &bufsize, stdin);
	return line;
}

void tokenize(void)
{
	int i = 0;
}

void runShell(void)
{
	readLine();
	printf(line);
}

int main(void)
{
	runShell();

	return 0;
}