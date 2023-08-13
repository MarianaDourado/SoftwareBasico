#include <stdio.h>

#include "../globals/global.h"
#include "../helpers/helper.h"
#include "array.h"

void processElementAccess(char *line)
{
	fprintf(file, "\t# %s\n", line);

	char arrayVariable[5];
	char destinationVariable[5];
	int elementIndex;

	sscanf(line, "get %s index ci%d to %s", arrayVariable, &elementIndex, destinationVariable);

	fprintf(file, "\tmovabs $%d, %%rcx\n", elementIndex);
	fprintf(file, "\timulq $4, %%rcx\n");
	fprintf(file, "\taddq %s, %%rcx\n", getValueFromMap(variableMap, arrayVariable));
	fprintf(file, "\tmovl (%%rcx), %%eax\n");
	fprintf(file, "\tmovl %%eax, %s\n\n", getValueFromMap(variableMap, destinationVariable));
}

void processElementAssignment(char *line)
{
	fprintf(file, "\t# %s\n", line);

	char destinationArray[5];
	char valueVariable[5];
	int elementIndex;

	sscanf(line, "set %s index ci%d with %s", destinationArray, &elementIndex, valueVariable);

	int positionIndex = elementIndex * 4;

	if (startsWith(destinationArray, "v"))
	{
		fprintf(file, "\tleaq %s, %%rcx\n", getValueFromMap(variableMap, destinationArray));
	}
	else
	{
		fprintf(file, "\tmovq %s, %%rcx\n", getValueFromMap(variableMap, destinationArray));
	}

	char position[5] = "";

	if (positionIndex)
	{
		sprintf(position, "%d", positionIndex);
	}

	if (startsWith(valueVariable, "ci"))
	{
		int constantValue;
		sscanf(valueVariable, "ci%d", &constantValue);
		fprintf(file, "\tmovl $%d, %%eax\n", constantValue);
	}
	else
	{
		fprintf(file, "\tmovl %s, %%eax\n", getValueFromMap(variableMap, valueVariable));
	}

	fprintf(file, "\tmovl %%eax, %s(%%rcx)\n\n", position);
}