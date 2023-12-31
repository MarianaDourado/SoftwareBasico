#include <stdio.h>
#include <string.h>

#include "../globals/global.h"
#include "../helpers/helper.h"
#include "function.h"

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
	char variable[6];
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
	fprintf(file, "\t# %s\n", line);

	char returnValue[20];
	sscanf(line, "return %s", returnValue);

	if (startsWith(returnValue, "ci"))
	{
		int constant;
		sscanf(returnValue, "ci%d", &constant);
		fprintf(file, "\tmovl $%d, %%eax\n", constant);
	}
	else
	{
		fprintf(file, "\tmovl %s, %%eax\n", getValueFromMap(variableMap, returnValue));
	}

	fprintf(file, "\n");
}

void processFunctionEnd()
{
	for (int i = 0; i < registerIndex; i++)
	{
		char registerName[6];
		sscanf(variableRegisters[i], "%%%[^d]d", registerName);
		fprintf(file, "\tmovq %s, %%%s\n", getValueFromMap(variableMap, registerName), registerName);
	}

	fprintf(file, "\n\tleave\n");
	fprintf(file, "\tret\n\n");

	stackSize = 0;
	registerIndex = 0;
	emptyMap(&variableMap);
}