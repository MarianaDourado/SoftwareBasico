#include <stdio.h>
#include <string.h>
#include "variables.h"

void process_local_variables(char line[256], int tam_pilha, int *addrs) // MARI
{
  FILE *f;
  f = fopen("file.S", "a+");

  char tipo[4];
  int i_a_or_r, countVar, size_vet;

  // declara
  sscanf(line, "%s v%c%d size ci%d", &tipo, &i_a_or_r, &countVar, &size_vet);

  // declara variavel
  if (strncmp(tipo, "var", 3) == 0)
  {
    i_a_or_r = 'i';
    tam_pilha += 4;
    fprintf(f, "\t# v%c%d -> %d(%%rbp)\n", i_a_or_r, countVar, -tam_pilha);
  }

  // declara registrador
  else if (strcmp(tipo, "reg") == 0)
  {
    i_a_or_r = 'r';
    tam_pilha += 4;
    fprintf(f, "\t# v%c%d -> %d(%%rbp)\n", i_a_or_r, countVar, -tam_pilha);
  }

  // declara vetor
  else if (strcmp(tipo, "vet") == 0)
  {
    i_a_or_r = 'a';
    (tam_pilha) += 4 * size_vet;
    int aux = -tam_pilha;
    for (int i = 0; i < size_vet; i++, aux += 4)
    {
      fprintf(f, "\t# v%c%d[%d] -> %d(%%rbp)\n", i_a_or_r, countVar, i, aux);
    }
  }

  fclose(f);
}