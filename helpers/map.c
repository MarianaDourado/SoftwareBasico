#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

void addToMap(KeyValuePair **map, const char *variable, char *assemblyReference)
{
	KeyValuePair *newPair = (KeyValuePair *)malloc(sizeof(KeyValuePair));
	if (newPair == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	// newPair->size = 4;
	strcpy(newPair->variable, variable);
	strcpy(newPair->assemblyReference, assemblyReference);
	newPair->next = NULL;

	if (*map == NULL)
	{
		*map = newPair;
	}
	else
	{
		struct KeyValuePair *current = *map;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newPair;
	}
}

char *getValueFromMap(KeyValuePair *map, const char *variable)
{
	while (map != NULL)
	{
		if (strcmp(map->variable, variable) == 0)
		{
			return map->assemblyReference;
		}
		map = map->next;
	}
	fprintf(stderr, "Key not found: %s\n", variable);
	exit(EXIT_FAILURE);
}

void emptyMap(KeyValuePair **map)
{
	while (*map != NULL)
	{
		struct KeyValuePair *temp = *map;
		*map = (*map)->next;
		free(temp);
	}
}

void freeMap(KeyValuePair *map)
{
	while (map != NULL)
	{
		KeyValuePair *temp = map;
		map = map->next;
		free(temp);
	}
}
