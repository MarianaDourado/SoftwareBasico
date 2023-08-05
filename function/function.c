#include <stdio.h>

#include "../globals/global.h"

void processFunctionDeclaration(char *line)
{
	int functionIndex;
	char typeOfParameter1, typeOfParameter2, typeOfParameter3;
	int numberOfVariablesFilled = sscanf(line, "function f%d p%c1 p%c2 p%c3", &functionIndex, &typeOfParameter1, &typeOfParameter2, &typeOfParameter3);

	fprintf(file, ".globl f%d\n", functionIndex);
	fprintf(file, "f%d:\n", functionIndex);

	fprintf(file, "\tpushq %%rbp\n");
	fprintf(file, "\tmovq %%rsp, %%rbp\n\n");
}

void processFunctionReturn(char *line)
{
}

void processFunctionEnd()
{
	fprintf(file, "\n\tleave\n");
	fprintf(file, "\tret\n\n");
}