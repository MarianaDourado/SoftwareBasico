#include <stdio.h>
#include <string.h>
#include "if.h"

void process_if(char line[256], int countIf)
{
  FILE *f;
  f = fopen("file.S", "a+");

  char v_p_or_c, cond[4];
  int co1, co2;

  int r = sscanf(line, "if %ci%d %s %ci%d", &v_p_or_c, &co1, &cond, &v_p_or_c, &co2);
}