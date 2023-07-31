#include <stdio.h>

#include "../helper/helper.h"

void processIf(char *line, int count, int leftParameter, int rightParameter)
{
	printLine(line, count);
	printf("Indices: %d e %d\n", leftParameter, rightParameter);
	printf("---\n");
}