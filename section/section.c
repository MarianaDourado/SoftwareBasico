#include <stdio.h>

#include "../globals/global.h"

void addData()
{
	fprintf(file, ".data\n\n");
}

void addRoData()
{
	fprintf(file, ".section .rodata\n\n");
}

void addText()
{
	fprintf(file, ".text\n\n");
}
