#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../globals/global.h"
#include "../helpers/helper.h"

char *processRelationalOperator(char *operator)
{
	char *instruction = (char *)malloc(4); // Allocate memory for instruction
	if (instruction == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	instruction[0] = 'j';
	switch (operator[0])
	{
	case 'e':
		strcat(instruction, "ne");
		break;
	case 'n':
		strcat(instruction, "e");
		break;
	case 'l':
		strcat(instruction, "g");
		break;
	case 'g':
		strcat(instruction, "l");
		break;
	}

	if (operator[0] == 'g' || operator[0] == 'l')
	{
		switch (operator[1])
		{
		case 't':
			strcat(instruction, "e");
			break;
		case 'e':
			break;
		}
	}

	return instruction;
}

void processConditional(char *line)
{
	fprintf(file, "\t# %s\n", line);

	char operator[3];
	char parameters[2][20];
	int numberOfVariablesFilled = sscanf(line, "if %s %s %s", parameters[0], operator, parameters[1]);

	char assemblyReferences[2][20];
	for (int i = 0; i < 2; i++)
	{
		if (startsWith(parameters[i], "ci"))
		{
			int constantValue;
			sscanf(parameters[i], "ci%d", &constantValue);

			if (i == 1)
			{
				fprintf(file, "\tmovl $%d, %%ecx\n", constantValue);
				sprintf(assemblyReferences[i], "%%ecx");
			}
			else
			{
				sprintf(assemblyReferences[i], "$%d", constantValue);
			}
		}
		else
		{
			strcpy(assemblyReferences[i], getValueFromMap(variableMap, parameters[i]));
		}
	}

	fprintf(file, "\tcmpl %s, %s\n", assemblyReferences[1], assemblyReferences[0]);
	fprintf(file, "\t%s endif%d\n\n", processRelationalOperator(operator), currentConditional);
}

void processConditionalEnd()
{
	fprintf(file, "endif%d:\n", currentConditional++);
}