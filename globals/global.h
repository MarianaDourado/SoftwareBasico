#pragma once

#include <stdio.h>

#include "../helpers/map.h"

extern FILE *file;
extern KeyValuePair *variableMap;
extern char *variableRegisters[];
extern char *integerRegisters[];
extern char *parameterRegisters[];

extern int stackSize;
extern int registerIndex;
extern int numberOfParameters;
extern int currentConditional;
