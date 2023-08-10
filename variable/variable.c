#include <stdio.h>
#include <string.h>

#include "../globals/global.h"
#include "../helpers/helper.h"

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

void processVariableDeclarations(char *line)
{
}

void processVariableDeclarationsEnd(char *line)
{
	KeyValuePair *tempMap = variableMap;

	if (stackSize % 8 != 0)
	{
		stackSize += 4;
	}

	for (int i = 0; i < numberOfParameters; i++)
	{
		stackSize += 8;
		char stackPosition[20];
		sprintf(stackPosition, "-%d(%%rbp)", stackSize);
		char registerName[4];
		sscanf(parameterRegisters[i], "%%%s", registerName);
		addToMap(&variableMap, registerName, stackPosition);
	}

	while (tempMap != NULL)
	{
		if (!startsWith(tempMap->variable, "vr") && !startsWith(tempMap->variable, "p"))
		{
			char position[5];

			int i;
			for (i = 0; tempMap->assemblyReference[i] != '(' && i < strlen(tempMap->assemblyReference); i++)
			{
				position[i] = tempMap->assemblyReference[i];
			}

			position[i] = '\0';

			fprintf(file, "\t# %s: %s\n", tempMap->variable, position);
		}

		tempMap = tempMap->next;
	}

	int stackAlignment = 0;
	while (stackAlignment < stackSize)
	{
		stackAlignment += 16;
	}

	if (stackSize)
	{
		fprintf(file, "\tsubq $%d, %%rsp\n\n", stackAlignment);
	}
}

void processArrayDeclaration(char *line)
{
	int variableIndex;
	int arraySize;
	sscanf(line, "vet va%d size ci%d", &variableIndex, &arraySize);

	char variable[5];
	sprintf(variable, "va%d", variableIndex);

	stackSize += 4 * arraySize;
	if (stackSize % 8 != 0)
	{
		stackSize += 4;
	}

	char stackPosition[20];
	sprintf(stackPosition, "-%d(%%rbp)", stackSize);

	addToMap(&variableMap, variable, stackPosition);
}

void processStackVariableDeclaration(char *line)
{
	int variableIndex;
	sscanf(line, "var vi%d", &variableIndex);
	char variable[5];
	sprintf(variable, "vi%d", variableIndex);

	stackSize += 4;

	char stackPosition[20];
	sprintf(stackPosition, "-%d(%%rbp)", stackSize);

	addToMap(&variableMap, variable, stackPosition);
}

void processRegisterVariableDeclaration(char *line)
{
	int variableIndex;
	sscanf(line, "reg vr%d", &variableIndex);

	char variable[5];
	sprintf(variable, "vr%d", variableIndex);

	addToMap(&variableMap, variable, variableRegisters[registerIndex++]);
}

void saveRegisters()
{
	for (int i = 0; i < numberOfParameters; i++)
	{
		char registerName[4];
		sscanf(parameterRegisters[i], "%%%s", registerName);
		fprintf(file, "\tmovq %%%s, %s\n", registerName, getValueFromMap(variableMap, registerName));
	}
}

void retrieveRegisters()
{
	for (int i = 0; i < numberOfParameters; i++)
	{
		char registerName[4];
		sscanf(parameterRegisters[i], "%%%s", registerName);
		fprintf(file, "\tmovq %s, %%%s\n", getValueFromMap(variableMap, registerName), registerName);
	}
}

void processCallParameterAssignment(char *parameter, int parameterIndex)
{
	if (startsWith(parameter, "ci"))
	{
		int constant;
		sscanf(parameter, "ci%d", &constant);
		fprintf(file, "\tmovl $%d, %s\n", constant, integerRegisters[parameterIndex]);
	}
	else if (startsWith(parameter, "pa"))
	{
		char registerName[20];
		strcpy(registerName, getValueFromMap(variableMap, parameter));

		if (!startsWith(registerName, parameterRegisters[parameterIndex]))
		{
			fprintf(file, "\tmovq %s, %s\n", registerName, parameterRegisters[parameterIndex]);
		}
	}
	else
	{
		char registerName[20];
		strcpy(registerName, getValueFromMap(variableMap, parameter));

		if (!startsWith(registerName, integerRegisters[parameterIndex]))
		{
			fprintf(file, "\tmovl %s, %s\n", registerName, integerRegisters[parameterIndex]);
		}
	}
}

char *processOperation(char operator)
{
	switch (operator)
	{
	case '+':
		return "addl";
	case '-':
		return "subl";
	case '*':
		return "imull";
	}
}

void processVariableAssignment(char *line)
{
	fprintf(file, "\t# %s\n", line);

	int numberOfParametersFilled;
	char leftHandSide[10];
	char variableToAssign[10];
	char function[5];
	char parameters[3][4];

	numberOfParametersFilled = sscanf(line, "%[^call]call %s %s %s %s", leftHandSide, function, parameters[0], parameters[1], parameters[2]);
	sscanf(leftHandSide, "%[^= ]", variableToAssign);

	if (numberOfParametersFilled >= 2) // Chamada de função
	{
		saveRegisters();
		for (int i = 0; i < numberOfParametersFilled - 2; i++)
		{
			processCallParameterAssignment(parameters[i], i);
		}

		fprintf(file, "\tcall %s\n", function);
		fprintf(file, "\tmovl %%eax, %s\n", getValueFromMap(variableMap, variableToAssign));

		retrieveRegisters();
	}
	else // Atribuição
	{
		char operator;
		char variables[2][4];

		numberOfParametersFilled = sscanf(line, "%[^= ] = %[^ +-*/] %c %s", variableToAssign, variables[0], &operator, variables[1]);

		if (numberOfParametersFilled == 4) // Com operação
		{
			char assemblyReferences[2][20];
			for (int i = 0; i < 2; i++)
			{
				if (startsWith(variables[i], "ci"))
				{
					int constantValue;
					sscanf(variables[i], "ci%d", &constantValue);
					sprintf(assemblyReferences[i], "$%d", constantValue);
				}
				else
				{
					strcpy(assemblyReferences[i], getValueFromMap(variableMap, variables[i]));
				}
			}

			if (operator== '/')
			{
				char registerName[4] = "rdx";
				if (numberOfParameters == 3)
				{
					fprintf(file, "\tmovq %%%s, %s\n", registerName, getValueFromMap(variableMap, registerName));
				}

				fprintf(file, "\tmovl %s, %%eax\n", assemblyReferences[0]);
				fprintf(file, "\tmovl %s, %%ecx\n", assemblyReferences[1]);
				fprintf(file, "\tcltd\n");
				fprintf(file, "\tidivl %%ecx\n");

				if (numberOfParameters == 3)
				{
					fprintf(file, "\tmovq %s, %%%s\n", getValueFromMap(variableMap, registerName), registerName);
				}
			}
			else
			{
				fprintf(file, "\tmovl %s, %%eax\n", assemblyReferences[0]);
				fprintf(file, "\tmovl %s, %%ecx\n", assemblyReferences[1]);
				fprintf(file, "\t%s %%ecx, %%eax\n", processOperation(operator));
			}

			fprintf(file, "\tmovl %%eax, %s\n", getValueFromMap(variableMap, variableToAssign));
		}
		else // Simples
		{
			numberOfParametersFilled = sscanf(line, "%[^= ] = %s", variableToAssign, variables[0]);

			char assemblyReference[20];
			if (startsWith(variables[0], "ci"))
			{
				int constantValue;
				sscanf(variables[0], "ci%d", &constantValue);
				sprintf(assemblyReference, "$%d", constantValue);
			}
			else
			{
				strcpy(assemblyReference, getValueFromMap(variableMap, variables[0]));
			}

			fprintf(file, "\tmovl %s, %s\n", assemblyReference, getValueFromMap(variableMap, variableToAssign));
		}
	}

	fprintf(file, "\n");
}
