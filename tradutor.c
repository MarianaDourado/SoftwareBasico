#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256
#define boolean int

void removeNewline(char *line)
{
  while (*line)
  {
    if (*line == '\n')
    {
      *line = 0;
    }
    else
    {
      line++;
    }
  }
}

void printLine(char *line, int count)
{
  printf("Linha %d: %s\n", count, line);
}

void processEnd(char *line, int count)
{
  printLine(line, count);
  printf("Fim do arquivo\n");
  printf("---\n");
}

void processFor(char *line, int count, int numberOfVariablesFilled, char variable, int init, int end, int step)
{
  printLine(line, count);

  if (numberOfVariablesFilled == 4)
  {
    printf("Lido '%c' valores: %d, %d, %d\n", variable, init, end, step);
  }
  else if (numberOfVariablesFilled == 3)
  {
    printf("Lido '%c' valores: %d, %d\n", variable, init, end);
  }
  else if (numberOfVariablesFilled == 2)
  {
    printf("Lido '%c' valor: %d\n", variable, init);
  }

  printf("---\n");
}

void processIf(char *line, int count, int leftParameter, int rightParameter)
{
  printLine(line, count);
  printf("Indices: %d e %d\n", leftParameter, rightParameter);
  printf("---\n");
}

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
