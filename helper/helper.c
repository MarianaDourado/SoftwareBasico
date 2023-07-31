#include <stdio.h>

void removeNewline(char *line)
{
	while (*line)
	{
		if (*line == '\n')
		{
			*line = 0;
		}
		else
		{
			line++;
		}
	}
}

void printLine(char *line, int count)
{
	printf("Linha %d: %s\n", count, line);
}