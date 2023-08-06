struct KeyValuePair
{
	// int size;
	char variable[4];
	char assemblyReference[10];
	struct KeyValuePair *next;
};

typedef struct KeyValuePair KeyValuePair;

void addToMap(KeyValuePair **map, const char *variable, char *assemblyReference);
char *getValueFromMap(KeyValuePair *map, const char *variable);
void emptyMap(KeyValuePair **map);
void freeMap(KeyValuePair *map);