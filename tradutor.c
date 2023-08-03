#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256

// Remove o '\n' do fim da linha
void remove_newline(char *ptr) // BRUNO OK
{
  while (*ptr)
  {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}

int main()
{
  char line[256];
  int tam_pilha = 0;
  int vl_addrs[5] = {0}, param_addrs[3] = {0}, param_tipos[3] = {0};

  int countIf = 0;  // conta quantos ifs

  int *addrs; // salva endereços das variaveis na pulha

  initial_print();

  // Lê uma linha por vez
  while (fgets(line, LINESZ, stdin) != NULL)
  {
    remove_newline(line);

    // Verifica se line começa com 'end' (3 letras)
    if (strncmp(line, "end", 3) == 0)
    {
      // não é endif nem é enddef
      if ((strncmp(line, "endif", 5) != 0) &&
          (strncmp(line, "enddef", 6) != 0))
      {
        process_function_end();
        continue;
      }
    }

    // ---------------------- DEFINIÇÃO DE FUNÇÃO -------------------
    // verifica se a linha começa com 'function'
    if (strncmp(line, "function", 8) == 0)
    {
      for (int i = 0; i < 3; i++)
      {
        param_addrs[i] = 0;
        param_tipos[i] = 0;
      }
      process_function_def(line, param_tipos);
      continue;
    }

    // ------------- DEFINIÇÃO DE VARIÁVEIS LOCAIS --------------

    // verifica se a linha começa com 'def'
    if (strncmp(line, "def", 3) == 0)
    {
      fgets(line, LINESZ, stdin);
      remove_newline(line);

      tam_pilha = 0;

      for (int i = 0; i < 5; i++)
        vl_addrs[i] = 0;

      while (strncmp(line, "enddef", 6) != 0)
      {
        process_local_variables(line, &tam_pilha, vl_addrs);
        fgets(line, LINESZ, stdin);
        remove_newline(line);
      }

      // agora é 'enddef'
      aloca_pilha(&tam_pilha, param_tipos, param_addrs);

      continue;
    }

    // --------------------- ATRIBUIÇÃO --- CHAMADA DE FUNÇÃO -------------
    // verifica se a linha tem atribuição simples ou com chamada de função
    if (strstr(line, "=") != NULL)
    {
      if (strstr(line, "call") != NULL)
        process_call_attr(line, param_addrs, vl_addrs, param_tipos);
      else
        process_simple_attr(line, vl_addrs);
      continue;
    }
    // -------------------------- ACESSO ARRAY ------------------
    // verifica se é um get

    // verifica se é um set

    // -------------------------- CONDICIONAL ------------------

    // Verifica se é um 'if'
    if (strncmp(line, "if", 2) == 0)
    {
      countIf++;
      process_if(line, countIf);
      continue;
    }

    // -------------------------- RETORNO ------------------

    // Verifica se é um 'return'
    if (strncmp(line, "return", 6) != 0)
    {
      process_return(line, addrs);
    }
  }

  return 0;
}
