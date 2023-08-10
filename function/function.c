#include <stdio.h>
#include <string.h>

#include "../globals/global.h"
#include "../helpers/map.h"

void processFunctionDeclaration(char *line)
{
	fprintf(file, "# %s\n", line);

	int functionIndex;
	char parameterTypes[3];
	int numberOfVariablesFilled = sscanf(line, "function f%d p%c1 p%c2 p%c3", &functionIndex, &parameterTypes[0], &parameterTypes[1], &parameterTypes[2]);
	numberOfParameters = numberOfVariablesFilled - 1;

	fprintf(file, ".globl f%d\n", functionIndex);
	fprintf(file, "f%d:\n", functionIndex);

	fprintf(file, "\tpushq %%rbp\n");
	fprintf(file, "\tmovq %%rsp, %%rbp\n\n");

	// Parâmetros
	char variable[5];
	char assignedRegister[20];

	for (int i = 0; i < numberOfParameters; i++)
	{
		sprintf(variable, "p%c%d", parameterTypes[i], i + 1);

		strcpy(assignedRegister, parameterTypes[i] == 'i' ? integerRegisters[i] : parameterRegisters[i]);

		addToMap(&variableMap, variable, assignedRegister);
	}
}

void processFunctionReturn(char *line)
{
}

void processFunctionEnd()
{
	fprintf(file, "\tleave\n");
	fprintf(file, "\tret\n\n");

	stackSize = 0;
	registerIndex = 0;
	emptyMap(&variableMap);
}