#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "end/end.h"
#include "for/for.h"
#include "helper/helper.h"
#include "if/if.h"

#define LINESZ 256
#define boolean int

int main(int argc, char **argv)
{
  char line[LINESZ];
  int count = 0;

  // variáveis do sscanf
  int numberOfVariablesFilled;

  //  parâmetros do if
  int leftParameter, rightParameter;

  // parâmetros do for
  int init, end, step;

  char variable1;

  // Lê uma linha por vez
  while (fgets(line, LINESZ, stdin) != NULL)
  {
    count++;
    removeNewline(line);

    // Verifica se line começa com 'end' (3 letras)
    boolean isEnd = strncmp(line, "end", 3) == 0;
    if (isEnd)
    {
      processEnd(line, count);
      continue;
    }

    // Verifica se é 'for' e quanto elementos possui
    numberOfVariablesFilled = sscanf(line, "for %c = %d, %d, %d", &variable1, &init, &end, &step);
    boolean isFor = numberOfVariablesFilled > 1 && numberOfVariablesFilled < 5;
    if (isFor)
    {
      processFor(line, count, numberOfVariablesFilled, variable1, init, end, step);
      continue;
    }

    // Verifica se é um 'if'
    numberOfVariablesFilled = sscanf(line, "if v%d > v%d", &leftParameter, &rightParameter);
    boolean isIf = numberOfVariablesFilled == 2;
    if (isIf)
    {
      processIf(line, count, leftParameter, rightParameter);
      continue;
    }
  }

  return 0;
}
