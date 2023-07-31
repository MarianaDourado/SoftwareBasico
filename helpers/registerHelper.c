#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "helper.h"

char *callerSaveRegisters[] = {"rax", "rcx", "rdx", "rdi", "rsi", "r8", "r9", "r8", "r9", "r10", "r11"};
char *calleeSaveRegisters[] = {"rbx", "r12", "r13", "r14", "r15"};

char *getRegister(char *registerName, int targetSize)
{
	if (!contains(callerSaveRegisters, registerName) && !contains(calleeSaveRegisters, registerName))
	{
		return NULL;
	}

	if (targetSize == 8)
	{
		return registerName;
	}

	if (targetSize == 4)
	{
		char *result = (char *)malloc(5);
		if (isdigit(registerName[1]))
		{
			strcpy(result, registerName);
			strcat(result, "d");
			return result;
		}
		else
		{
			return replace(registerName, "r", "e");
		}
	}

	if (targetSize == 2)
	{
		char *result = (char *)malloc(5);
		if (isdigit(registerName[1]))
		{
			strcpy(result, registerName);
			strcat(result, "w");
			return result;
		}
		else
		{
			return replace(registerName, "r", "");
		}
	}

	char *result = (char *)malloc(5);
	if (isdigit(registerName[1]))
	{
		strcpy(result, registerName);
		strcat(result, "b");
		return result;
	}
	else if (registerName[2] == 'i' || registerName[2] == 'p')
	{
		strcpy(result, replace(registerName, "r", ""));
		strcat(result, "l");
		return result;
	}
	else
	{
		return replace(replace(registerName, "r", ""), "x", "l");
	}
}

char getSufix(int size)
{
	if (size == 8)
	{
		return 'q';
	}

	if (size == 4)
	{
		return 'l';
	}

	if (size == 2)
	{
		return 'w';
	}

	return 'b';
}