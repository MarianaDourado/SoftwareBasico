#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"

#define boolean int
#define true 1
#define false 0

void removeNewline(char *line)
{
	while (*line)
	{
		if (*line == '\n')
		{
			*line = 0;
		}
		else
		{
			line++;
		}
	}
}

void removeComments(char *line)
{
	while (*line)
	{
		if (*line == '#')
		{
			*line = 0;
		}
		else
		{
			line++;
		}
	}
}

void printLine(char *line, int count)
{
	printf("Linha %d: %s\n", count, line);
}

int contains(char **source, char *target)
{
	int i = 0;
	while (source[i])
	{
		if (strcmp(source[i], target) == 0)
		{
			return 1;
		}

		i++;
	}

	return 0;
}

char *replace(char *source, char *target, char *replacement)
{

	if (!source || !target)
	{
		return NULL;
	}

	int targetLength = strlen(target);
	if (targetLength == 0)
	{
		return NULL;
	}

	if (!replacement)
	{
		replacement = "";
	}
	int replacementLength = strlen(replacement);

	int numberOfReplacements;
	char *temporary;
	char *nextInsertPoint = source;
	for (numberOfReplacements = 0; temporary = strstr(nextInsertPoint, target); ++numberOfReplacements)
	{
		nextInsertPoint = temporary + targetLength;
	}

	char *result = malloc(strlen(source) + (replacementLength - targetLength) * numberOfReplacements + 1);
	if (!result)
	{
		return NULL;
	}

	temporary = result;

	int distanceBetweenTargetInstances;
	while (numberOfReplacements--)
	{
		nextInsertPoint = strstr(source, target);
		distanceBetweenTargetInstances = nextInsertPoint - source;
		temporary = strncpy(temporary, source, distanceBetweenTargetInstances) + distanceBetweenTargetInstances;
		temporary = strcpy(temporary, replacement) + replacementLength;
		source += distanceBetweenTargetInstances + targetLength;
	}

	strcpy(temporary, source);
	return result;
}

boolean startsWith(char *line, char *keyword)
{
	return strncmp(line, keyword, strlen(keyword)) == 0;
}