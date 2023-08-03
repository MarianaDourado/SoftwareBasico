#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256

char p1, p2, p3;
int r, i1, i2, i3;
int inicio, fim, passo;
char line[LINESZ];
int count = 0;

// variavel
char tipo[4];
char i_a_or_r, p1_i_or_a, p2_i_or_a, p3_i_or_a;
int size_vet;

char v_p_or_c;

// condicional
char cond[3];
int co1, co2;
int countIf = 0;

int r1; // para retorno
int size_vet;
int countVar = 0; // se pa isso aqui vai ter que ser dividido

int countFunc = 0;

int tam_pilha = 0, alinhamento, aux;
int *addrs; // salva endereços das variaveis na pulha

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

void initial_print() // MARI OK
{
  FILE *f = fopen("file.S", "a+");

  fprintf(f, ".data\n\n");
  fprintf(f, ".text\n");

  fclose(f);
}

void alinha_pilha(int t_pilha, int alinhamento) // RAFA VERIFICA
{
  FILE *f;
  f = fopen("file.S", "a+");

  // ve se ta alinhado
  if (t_pilha < alinhamento)
    t_pilha += (alinhamento - t_pilha);
  t_pilha += (t_pilha % alinhamento);

  fprintf(f, "\tsubq $%d, %%rsp\n", tam_pilha);

  fclose(f);
}

void aloca_pilha() // RAFA
{
  // ......

  alinha_pilha(tam_pilha, 16);
}

void process_function_init(int cF) // MARI
{
  FILE *f;
  f = fopen("file.S", "a+");

  fprintf(f, ".globl f%d\n", cF);
  fprintf(f, "f%d:\n", cF);

  fprintf(f, "\tpushq %%rbp\n");
  fprintf(f, "\tmovq %%rsp, %%rbp\n\n");

  fclose(f);
}

void process_function_def(char line[256], int *param_types) // MARI
{
  FILE *f;
  f = fopen("file.S", "a+");

  alinhamento = 4;

  r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &countFunc, &p1_i_or_a, &p1, &p2_i_or_a, &p2, &p3_i_or_a, &p3);

  process_function_init(countFunc);

  switch (r)
  {
  // não tem parametros
  case 1:
    break;

  // tem 1 parâmetro
  case 3:
    if (p1_i_or_a == 'a')
    {
      tam_pilha += 8;
      alinhamento = 8;
    }
    else if (p1_i_or_a == 'i')
      tam_pilha += 4;
    // NÃO SEI COMO ALINHAR ISSSOOOOOOOOOOOOOOO!!!!!!!!!
    // exemplo: i i i i - - - - j j j j j j j j

    tam_pilha += 8;
    fprintf(f, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
    break;

  // tem 2 parâmetros
  case 5:
    // ...

    tam_pilha += 8;
    fprintf(f, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
    tam_pilha += 8;
    fprintf(f, "\t# p%c%d -> %d(%%rbp)\n", p2_i_or_a, p2, -tam_pilha);
    break;

  // tem 3 parâmetros
  case 7:
    if (p1_i_or_a == 'a')
      tam_pilha += 8;
    else if (p1_i_or_a == 'i')
      tam_pilha += 4;
    //...

    fprintf(f, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
    tam_pilha += 8;
    fprintf(f, "\t# p%c%d -> %d(%%rbp)\n", p2_i_or_a, p2, -tam_pilha);
    tam_pilha += 8;
    fprintf(f, "\t# p%c%d -> %d(%%rbp)\n", p3_i_or_a, p3, -tam_pilha);
    break;

  default:
    fprintf(f, "\tfalha na declaracao da funcao\n");
    break;
  }

  fclose(f);
}

void process_local_variables(char line[256], int tam_pilha, int *addrs) // MARI
{
  FILE *f;
  f = fopen("file.S", "a+");

  // declara
  sscanf(line, "%s v%c%d size ci%d", &tipo, &i_a_or_r, &countVar, &size_vet);

  // declara vetor
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
    fprintf(f, "\t# r%c%d -> %d(%%rbp)\n", i_a_or_r, countVar, -tam_pilha);
  }

  // declara vetor
  else if (strcmp(tipo, "vet") == 0)
  {
    i_a_or_r = 'a';
    (tam_pilha) += 4 * size_vet;
    aux = -tam_pilha;
    for (int i = 0; i < size_vet; i++, aux += 4)
    {
      fprintf(f, "\t# v%c%d[%d] -> %d(%%rbp)\n", i_a_or_r, countVar, i, aux);
    }
  }

  fclose(f);
}

void process_function_end() // MARI
{
  FILE *f;
  f = fopen("file.S", "a+");

  // retorna os registradores ?????????????????????????????

  fprintf(f, "\tleave\n");
  fprintf(f, "\tret\n\n");
  fclose(f);

  // alguma variável precisa ser reinicializada?
}

void process_return(char line[256], int *addrs) // MARI
{
  FILE *f;
  int ret; // para armazenar o  valor inteiro do retorno
           //}
  int t;
  char registrador[4] = "";
  char menos;

  f = fopen("file.S", "a+");
  t = sscanf(line, "return %c%c%c%d", &menos, &v_p_or_c, &i_a_or_r, &ret);

  fprintf(f, "\n\t# %s\n", line);

  if (v_p_or_c == 'p')
  {
    if (ret == 1)
      strcpy(registrador, "edi");
    if (ret == 2)
      strcpy(registrador, "esi");
    if (ret == 3)
      strcpy(registrador, "edx");

    fprintf(f, "\tmovl %%", registrador, ", %eax\n");
  }
  else if (v_p_or_c == 'v')
  {
    // pode ser 'vi' ou 'vr'
    fprintf(f, "\tmovl %d(%%rbp), %%eax\n", addrs[ret - 1]);
  }
  else if (v_p_or_c == 'c')
  {
    if (menos == '-')
      fprintf(f, "\tmovl $-", ret, ", %eax\n");
    else
      fprintf(f, "\tmovl $", ret, ", %eax\n");
  }
  fclose(f);
}

void process_simple_attr(char line[256], int *addrs) // RAFA
{

  FILE *fp = fopen("file.S", "a+");
  char destType, signal = '0', p1_v_p_or_c, p2_v_p_or_c;
  int countVar, param1Index, param2Index;

  sscanf(line, "%ci%d = %ci%d %c %ci%d", &destType, &countVar, &p1_v_p_or_c, &param1Index, &signal, &p2_v_p_or_c, &param2Index);

  fprintf(fp, "\n\t# %s\n", line);

  if (signal == '0')
  {
    switch (p1_v_p_or_c)
    {
    case 'v':
      fprintf(fp, "\tmovl %d(%%rbp), %%eax\n", addrs[param1Index - 1]);
      fprintf(fp, "\tmovl %%eax, ");
      break;
    case 'p':
      switch (param1Index)
      {
      case 1:
        fprintf(fp, "\tmovl %%edi, ");
        break;
      case 2:
        fprintf(fp, "\tmovl %%esi, ");
        break;
      case 3:
        fprintf(fp, "\tmovl %%edx, ");
        break;
      }
      break;
    case 'c':
      fprintf(fp, "\tmovl $%d, ", param1Index);
      break;
    }
    fprintf(fp, "%d(%%rbp)\n", addrs[countVar - 1]);
  }
  else if (signal == '/')
  {
    switch (p1_v_p_or_c)
    {
    case 'v':
      fprintf(fp, "\tmovl %d(%%rbp), %%eax\n", addrs[param1Index - 1]);
      break;
    case 'p':
      switch (param1Index)
      {
      case 1:
        fprintf(fp, "\tmovl %%edi, %%eax\n");
        break;
      case 2:
        fprintf(fp, "\tmovl %%esi, %%eax\n");
        break;
      case 3:
        fprintf(fp, "\tmovl %%edx, %%eax\n");
        break;
      }
      break;
    case 'c':
      fprintf(fp, "\tmovl $%d, %%eax\n", param1Index);
      break;
    }
    // extendendo o eax para edx
    fprintf(fp, "\tcltd\n");
    switch (p2_v_p_or_c)
    {
    case 'v':
      fprintf(fp, "\tidivl %d(%%rbp)\n", addrs[param2Index - 1]);
      break;
    case 'p':
      switch (param2Index)
      {
      case 1:
        fprintf(fp, "\tidivl %%edi\n");
        break;
      case 2:
        fprintf(fp, "\tidivl %%esi\n");
        break;
      case 3:
        fprintf(fp, "\tidivl %%edx\n");
        break;
      }
      break;
    case 'c':
      fprintf(fp, "\tmovl $%d, %%ecx\n", param2Index);
      fprintf(fp, "\tidivl %%ecx\n");
      break;
    }
    fprintf(fp, "\tmovl %%eax, %d(%%rbp)\n", addrs[countVar - 1]);
  }
  else
  {
    switch (p1_v_p_or_c)
    {
    case 'v':
      fprintf(fp, "\tmovl %d(%%rbp), %%eax\n", addrs[param1Index - 1]);
      break;
    case 'p':
      switch (param1Index)
      {
      case 1:
        fprintf(fp, "\tmovl %%edi, %%eax\n");
        break;
      case 2:
        fprintf(fp, "\tmovl %%esi, %%eax\n");
        break;
      case 3:
        fprintf(fp, "\tmovl %%edx, %%eax\n");
        break;
      }
      break;
    case 'c':
      fprintf(fp, "\tmovl $%d, %%eax\n", param1Index);
      break;
    }
    switch (signal)
    {
    case '+':
      switch (p2_v_p_or_c)
      {
      case 'v':
        fprintf(fp, "\taddl %d(%%rbp), %%eax", addrs[param2Index - 1]);
        break;
      case 'p':
        switch (param2Index)
        {
        case 1:
          fprintf(fp, "\taddl %%edi, %%eax\n");
          break;
        case 2:
          fprintf(fp, "\taddl %%esi, %%eax\n");
          break;
        case 3:
          fprintf(fp, "\taddl %%edx, %%eax\n");
          break;
        }
        break;
      case 'c':
        fprintf(fp, "\taddl $%d, %%eax\n", param2Index);
        break;
      }
      break;
    case '-':
      switch (p2_v_p_or_c)
      {
      case 'v':
        fprintf(fp, "\tsubl %d(%%rbp), %%eax\n", addrs[param2Index - 1]);
        break;
      case 'p':
        switch (param2Index)
        {
        case 1:
          fprintf(fp, "\tsubl %%edi, %%eax\n");
          break;
        case 2:
          fprintf(fp, "\tsubl %%esi, %%eax\n");
          break;
        case 3:
          fprintf(fp, "\tsubl %%edx, %%eax\n");
          break;
        }
        break;
      case 'c':
        fprintf(fp, "\tsubl $%d, %%eax\n", param2Index);
        break;
      }
      break;
    case '*':
      switch (p2_v_p_or_c)
      {
      case 'v':
        fprintf(fp, "\timull %d(%%rbp), %%eax\n", addrs[param2Index - 1]);
        break;
      case 'p':
        switch (param2Index)
        {
        case 1:
          fprintf(fp, "\timull %%edi, %%eax\n");
          break;
        case 2:
          fprintf(fp, "\timull %%esi, %%eax\n");
          break;
        case 3:
          fprintf(fp, "\timull %%edx, %%eax\n");
          break;
        }
        break;
      case 'c':
        fprintf(fp, "\timull $%d, %%eax\n", param2Index);
        break;
      }
      break;
    }
    fprintf(fp, "\tmovl %%eax, %d(%%rbp)\n", addrs[countVar - 1]);
  }

  fclose(fp);
}

void process_call_attr(char line[256], int *addrsParams, int *addrVars, int *paramTypes)
{
  FILE *f = fopen("file.S", "a+");

  char i_or_r, p1_v_p_or_c, p2_v_p_or_c, p3_v_p_or_c, p1_i_a_or_r, p2_i_a_or_r, param3i_a_or_r;
  int t, countVar, countFunc, p1_value, p2_value, p3_value;

  t = sscanf(line, "v%c%d = call f%d %c%c%d %c%c%d %c%c%d", &i_or_r, &countVar, &countFunc, &p1_v_p_or_c, &p1_i_a_or_r, &p1_value, &p2_v_p_or_c, &p2_i_a_or_r, &p2_value, &p3_v_p_or_c, &param3i_a_or_r, &p3_value);

  if (paramTypes[0])
    fprintf(f, "\n\t# salvando parametros\n");

  for (int i = 0; paramTypes[i]; i++)
  {
    switch (i)
    {
    case 0:
      fprintf(f, "\tmovq %%rdi, %d(%%rbp)\n", addrsParams[i]);
      break;
    case 1:
      fprintf(f, "\tmovq %%rsi, %d(%%rbp)\n", addrsParams[i]);
      break;
    case 2:
      fprintf(f, "\tmovq %%rdx, %d(%%rbp)\n", addrsParams[i]);
    }
  }

  fprintf(f, "\n\t# %s\n", line);

  // se não tem parâmetro so faz o call direto

  if (t >= 6)
  {
    switch (p1_v_p_or_c)
    {
    case 'v':
      switch (p1_i_a_or_r)
      {
      case 'i':
        fprintf(f, "\tmovl %d(%%rbp), %%edi\n", addrVars[p1_value - 1]);
        break;
      case 'a':
        fprintf(f, "\tleaq %d(%%rbp), %%rdi\n", addrVars[p1_value - 1]);
        break;
      case 'r': //???????????????????????????????
        fprintf(f, "\tmovl %d(%%rbp), %%edi\n", addrVars[p1_value - 1]);
        break;
      }
      break;
    case 'p': // MARIANA PAROU AQUI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
      switch (p1_i_a_or_r)
      {
      case 'i':
        if (p1_value == 1)
        {
          fprintf(f, "\tmovq %d(%%rbp), %%rax\n", addrsParams[1]);
          fprintf(f, "\tmovl %%eax, %%edi\n");
        }
        else if (p1_value == 2)
        {
          fprintf(f, "\tmovq %d(%%rbp), %%rax\n", addrsParams[2]);
          fprintf(f, "\tmovl %%eax, %%esi\n");
        }
        else if (p1_value == 3)
        {
          fprintf(f, "\tmovq %d(%%rbp), %%rax\n", addrsParams[2]);
          fprintf(f, "\tmovl %%eax, %%edi\n");
        }
        break;
      case 'a':
        if (p1_value == 1)
        {
          fprintf(f, "\tleaq %d(%%rbp), %%rdi\n", addrsParams[1]);
        }
        else if (p1_value == 2)
        {
          fprintf(f, "\tleaq %d(%%rbp), %%rdi\n", addrsParams[1]);
        }
        else if (p1_value == 3)
        {
          fprintf(f, "\tleaq %d(%%rbp), %%rdi\n", addrsParams[2]);
        }
        break;
      }
      break;
    case 'c':
      fprintf(f, "\tmovl $%d, %%edi\n", p1_value);
      break;
    }
  }
  if (t >= 9)
  {
    switch (p2_v_p_or_c)
    {
    case 'v':
      switch (p2_i_a_or_r)
      {
      case 'i':
        fprintf(f, "\tmovl %d(%%rbp), %%esi\n", addrVars[p2_value - 1]);
        break;
      case 'a':
        fprintf(f, "\tleaq %d(%%rbp), %%rsi\n", addrVars[p2_value - 1]);
        break;
      }
      break;
    case 'p':
      switch (p2_i_a_or_r)
      {
      case 'i':
        if (p2_value == 1)
        {
          fprintf(f, "\tmovq %d(%%rbp), %%rax\n", addrsParams[0]);
          fprintf(f, "\tmovl %%eax, %%esi\n");
        }
        else if (p2_value == 3)
        {
          fprintf(f, "\tmovq %d(%%rbp), %%rax\n", addrsParams[2]);
          fprintf(f, "\tmovl %%eax, %%esi\n");
        }
        break;
      case 'a':
        if (p2_value == 1)
        {
          fprintf(f, "\tleaq %d(%%rbp), %%rsi\n", addrsParams[0]);
        }
        else if (p2_value == 3)
        {
          fprintf(f, "\tleaq %d(%%rbp), %%rsi\n", addrsParams[2]);
        }
        break;
      }
      break;
    case 'c':
      fprintf(f, "\tmovl $%d, %%edi\n", p1_value);
      break;
    }
  }

  if (t == 12)
  {
    switch (p3_v_p_or_c)
    {
    case 'v':
      switch (param3i_a_or_r)
      {
      case 'i':
        fprintf(f, "\tmovl %d(%%rbp), %%edx\n", addrVars[p2_value - 1]);
        break;
      case 'a':
        fprintf(f, "\tleaq %d(%%rbp), %%edx\n", addrVars[p2_value - 1]);
        break;
      }
      break;
    case 'p':
      switch (param3i_a_or_r)
      {
      case 'i':
        if (p3_value == 1)
        {
          fprintf(f, "\tmovq %d(%%rbp), %%rax\n", addrsParams[0]);
          fprintf(f, "\tmovl %%eax, %%edx\n");
        }
        else if (p3_value == 2)
        {
          fprintf(f, "\tmovq %d(%%rbp), %%rax\n", addrsParams[1]);
          fprintf(f, "\tmovl %%eax, %%edx\n");
        }
        break;
      case 'a':
        if (p3_value == 1)
        {
          fprintf(f, "\tleaq %d(%%rbp), %%edx\n", addrsParams[0]);
        }
        else if (p3_value == 2)
        {
          fprintf(f, "\tleaq %d(%%rbp), %%edx\n", addrsParams[1]);
        }
        break;
      }
      break;
    case 'c':
      fprintf(f, "\tmovl $%d, %%edi\n", p1_value);
      break;
    }
  }

  fprintf(f, "\tcall f%d\n", countFunc);
  // movendo o resultado da função para o destino especificado
  fprintf(f, "\tmovl %%eax, %d(%%rbp)\n", addrVars[countVar - 1]);

  if (paramTypes[0])
    fprintf(f, "\n\t# recuperando parametros\n");

  for (int i = 0; paramTypes[i]; i++)
  {
    switch (i)
    {
    case 0:
      fprintf(f, "\tmovq %d(%%rbp), %%rdi\n", addrsParams[i]);
      break;
    case 1:
      fprintf(f, "\tmovq %d(%%rbp), %%rsi\n", addrsParams[i]);
      break;
    case 2:
      fprintf(f, "\tmovq %d(%%rbp), %%rdx\n", addrsParams[i]);
    }
  }

  fclose(f);
}

void process_if()
{ // GRILO
}

int main()
{
  char line[256];
  tam_pilha = 0;
  int vl_addrs[5] = {0}, param_addrs[3] = {0}, param_types[3] = {0};

  initial_print();

  // Lê uma linha por vez
  while (fgets(line, LINESZ, stdin) != NULL)
  {
    count++;
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
      // GabrielFR
      for (int i = 0; i < 3; i++)
      {
        param_addrs[i] = 0;
        param_types[i] = 0;
      }
      process_function_def(line, param_types);
      continue;
    }

    // ------------- DEFINIÇÃO DE VARIÁVEIS LOCAIS --------------

    // verifica se a linha começa com 'def'
    if (strncmp(line, "def", 3) == 0)
    {
      fgets(line, LINESZ, stdin);
      remove_newline(line);

      tam_pilha = 0;

      // GabrielFRails
      for (int i = 0; i < 5; i++)
      {
        vl_addrs[i] = 0;
      }

      while (strncmp(line, "enddef", 6) != 0)
      {
        process_local_variables(line, &tam_pilha, vl_addrs);
        fgets(line, LINESZ, stdin);
        remove_newline(line);
      }

      // agora é 'enddef'
      aloca_pilha(&tam_pilha, param_types, param_addrs);

      continue;
    }

    // --------------------- ATRIBUIÇÃO --- CHAMADA DE FUNÇÃO -------------
    // verifica se a linha tem atribuição simples ou com chamada de função
    if (strstr(line, "=") != NULL)
    {

      if (strstr(line, "call") != NULL)
        process_call_attr(line, param_addrs, vl_addrs, param_types);
      else
        process_simple_attr(line, vl_addrs);

      continue;
    }
    // -------------------------- ACESSO ARRAY ------------------

    // -------------------------- CONDICIONAL ------------------

    // Verifica se é um 'if'
    r = sscanf(line, "if %ci%d %s %ci%d", &v_p_or_c, &co1, &cond, &v_p_or_c, &co2);
    if (r)
    {
      countIf++;
      process_if();
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
