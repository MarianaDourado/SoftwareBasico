#include <stdio.h>
#include <string.h>
#include "pilha.h"

void alinha_pilha(int t_pilha, int alinhamento) // RAFA VERIFICA
{
  FILE *f;
  f = fopen("file.S", "a+");

  // ve se ta alinhado
  if (t_pilha < alinhamento)
    t_pilha += (alinhamento - t_pilha);
  t_pilha += (t_pilha % alinhamento);

  fprintf(f, "\tsubq $%d, %%rsp\n", t_pilha);

  fclose(f);
}

void aloca_pilha() // RAFA
{
  int t_pilha = 0;
  // ......

  alinha_pilha(t_pilha, 16);
}