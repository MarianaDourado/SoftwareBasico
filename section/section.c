#include <stdio.h>

#include "../globals/global.h"
#include "section.h"

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
