#include <stdio.h>
#include <string.h>

#include "../globals/global.h"
#include "../helpers/helper.h"

void processConditional(char *line)
{
	char operator[3];
	char leftParameterType, rightParameterType;
	int leftParameterIndex, rightParameterIndex;
	int numberOfVariablesFilled = sscanf(line, "if %ci%d %s %ci%d", &leftParameterType, &leftParameterIndex, operator, & rightParameterType, &rightParameterIndex);

	char instruction[3] = "j";
	switch (operator[0])
	{
	case 'e':
		strcat(instruction, "e");
		break;
	case 'n':
		strcat(instruction, "ne");
		break;
	case 'l':
		strcat(instruction, "l");
		break;
	case 'g':
		strcat(instruction, "g");
		break;
	}

	if (operator[0] == 'g' || operator[0] == 'l')
	{
		switch (operator[1])
		{
		case 't':
			break;
		case 'e':
			strcat(instruction, "e");
			break;
		}
	}

	// TODO: Passar parâmetros em Assembly
	fprintf(file, "\tcmpl %ci%d, %ci%d\n", rightParameterType, rightParameterIndex, leftParameterType, leftParameterIndex);
	fprintf(file, "\t%s label\n", instruction);
}

void processConditionalEnd()
{
	fprintf(file, "\tendif\n");
}