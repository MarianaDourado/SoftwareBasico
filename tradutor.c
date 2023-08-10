#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array/array.h"
#include "conditional/conditional.h"
#include "function/function.h"
#include "globals/global.h"
#include "helpers/helper.h"
#include "variable/variable.h"
#include "section/section.h"

#define LINESZ 256
#define boolean int

FILE *file;
KeyValuePair *variableMap;
char *variableRegisters[] = {"%r8d", "%r9d", "%r10d", "%r11d"};
char *parameterRegisters[] = {"%rdi", "%rsi", "%rdx"};
char *integerRegisters[] = {"%edi", "%esi", "%edx"};

int stackSize = 0;
int registerIndex = 0;
int numberOfParameters = 0;

int main(int argc, char **argv)
{
  char line[LINESZ];
  int lineNumber = 0;

  file = fopen("output/file.S", "w");

  addData();
  addText();

  // Lê uma linha por vez
  while (fgets(line, LINESZ, stdin) != NULL)
  {
    lineNumber++;
    removeNewline(line);
    // removeComments(line);

    // ------------------------- VARIÁVEIS ---------------------------
    if (startsWith(line, "def"))
    {
      processVariableDeclarations(line);
      continue;
    }

    if (startsWith(line, "enddef"))
    {
      processVariableDeclarationsEnd(line);
      continue;
    }

    // DECLARAÇÃO
    if (startsWith(line, "var"))
    {
      processStackVariableDeclaration(line);
      continue;
    }

    if (startsWith(line, "vet"))
    {
      processArrayDeclaration(line);
      continue;
    }

    if (startsWith(line, "reg"))
    {
      processRegisterVariableDeclaration(line);
      continue;
    }

    // ATRIBUIÇÃO
    if (startsWith(line, "v"))
    {
      processVariableAssignment(line);
      continue;
    }

    // --------------------------- ARRAYS ----------------------------
    if (startsWith(line, "get"))
    {
      processElementAccess(line);
      continue;
    }

    if (startsWith(line, "set"))
    {
      processElementAssignment(line);
      continue;
    }

    // ------------------------- CONDICIONAIS -----------------------
    if (startsWith(line, "if"))
    {
      processConditional(line);
      continue;
    }

    if (startsWith(line, "endif"))
    {
      processConditionalEnd(file);
      continue;
    }

    // --------------------------- FUNÇÕES --------------------------
    if (startsWith(line, "function"))
    {
      processFunctionDeclaration(line);
      continue;
    }

    if (startsWith(line, "end"))
    {
      processFunctionEnd();
      continue;
    }

    if (startsWith(line, "return"))
    {
      processFunctionReturn(line);
      continue;
    }
  }

  freeMap(variableMap);
  fclose(file);
  return 0;
}
