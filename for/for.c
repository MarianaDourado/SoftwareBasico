#include <stdio.h>

#include "../helpers/helper.h"

void processFor(char *line, int count, int numberOfVariablesFilled, char variable, int init, int end, int step)
{
	printLine(line, count);

	if (numberOfVariablesFilled == 4)
	{
		printf("Lido '%c' valores: %d, %d, %d\n", variable, init, end, step);
	}
	else if (numberOfVariablesFilled == 3)
	{
		printf("Lido '%c' valores: %d, %d\n", variable, init, end);
	}
	else if (numberOfVariablesFilled == 2)
	{
		printf("Lido '%c' valor: %d\n", variable, init);
	}

	printf("---\n");
}