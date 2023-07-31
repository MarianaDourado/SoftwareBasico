#include <stdio.h>

#include "../helpers/helper.h"

void processEnd(char *line, int count)
{
	printLine(line, count);
	printf("Fim do arquivo\n");
	printf("---\n");
}