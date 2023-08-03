#include <stdio.h>
#include <string.h>
#include "prints.h"

void initial_print() // MARI OK
{
  FILE *f = fopen("file.S", "a+");

  fprintf(f, ".data\n\n");
  fprintf(f, ".text\n");

  fclose(f);
}