#pragma once

struct KeyValuePair
{
	char variable[6];
	char assemblyReference[20];
	struct KeyValuePair *next;
};

typedef struct KeyValuePair KeyValuePair;

void addToMap(KeyValuePair **map, const char *variable, char *assemblyReference);
char *getValueFromMap(KeyValuePair *map, const char *variable);
void emptyMap(KeyValuePair **map);
void freeMap(KeyValuePair *map);