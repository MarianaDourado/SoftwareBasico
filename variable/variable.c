#include <stdio.h>
#include <string.h>

#include "../globals/global.h"
#include "../helpers/helper.h"

void processVariableDeclarations(char *line)
{
}

void processVariableDeclarationsEnd(char *line)
{
	int stackAlignment = 0;
	while (stackAlignment < stackSize)
	{
		stackAlignment += 16;
	}

	KeyValuePair *tempMap = variableMap;
	while (tempMap != NULL)
	{
		if (!startsWith(tempMap->variable, "vr"))
		{
			char position[5];
			int i;

			for (i = 0; tempMap->assemblyReference[i] != '('; i++)
			{
				position[i] = tempMap->assemblyReference[i];
			}

			position[i] = '\0';

			fprintf(file, "\t# %s: %s\n", tempMap->variable, position);
		}

		tempMap = tempMap->next;
	}

	if (stackSize)
	{
		fprintf(file, "\t# rbx: -%d\n", stackSize);
		fprintf(file, "\tsubq $%d, %%rsp\n\n", stackAlignment);
	}
}

void processArrayDeclaration(char *line)
{
	int variableIndex;
	int arraySize;
	sscanf(line, "vet va%d size ci%d", &variableIndex, &arraySize);

	char variable[4];
	sprintf(variable, "va%d", variableIndex);

	stackSize += 4 * arraySize;

	char stackPosition[10];
	sprintf(stackPosition, "-%d(%%rbp)", stackSize);

	addToMap(&variableMap, variable, stackPosition);
}

void processStackVariableDeclaration(char *line)
{
	int variableIndex;
	sscanf(line, "var vi%d", &variableIndex);
	char variable[4];
	sprintf(variable, "vi%d", variableIndex);

	stackSize += 4;

	char stackPosition[10];
	sprintf(stackPosition, "-%d(%%rbp)", stackSize);

	addToMap(&variableMap, variable, stackPosition);
}

void processRegisterVariableDeclaration(char *line)
{
	int variableIndex;
	sscanf(line, "reg vr%d", &variableIndex);

	char variable[4];
	sprintf(variable, "vr%d", variableIndex);

	addToMap(&variableMap, variable, variableRegisters[registerIndex++]);
}

void processVariableAssignment(char *line)
{
}