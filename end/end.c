#include <stdio.h>

#include "../helper/helper.h"

void processEnd(char *line, int count)
{
	printLine(line, count);
	printf("Fim do arquivo\n");
	printf("---\n");
}