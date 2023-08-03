#ifndef functions.h
#define functions.h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void process_function_init(int cF);

void process_function_def(char line[256], int *param_types);

void process_local_variables(char line[256], int tam_pilha, int *addrs);

void process_return(char line[256], int *addrs);


#endif