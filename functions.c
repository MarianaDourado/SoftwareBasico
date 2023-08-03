#include "functions.h"


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

void process_function_def(char line[256], int *tipos_parametros) // MARI
{
  FILE *f;
  f = fopen("file.S", "a+");

  int countFunc, p1_i_or_a, p1, p2_i_or_a, p2, p3_i_or_a, p3;

  int r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &countFunc, &p1_i_or_a, &p1, &p2_i_or_a, &p2, &p3_i_or_a, &p3);

  //tipos_parametros explanation:
  //nulo -> 0
  //inteiro -> 1
  //array -> 2


  if(r==7){ // total de 3 parametros
    // tem 1 parametro
    if (p1_i_or_a == 'a')
      tipos_parametros[0] = 2;
    else if (p1_i_or_a == 'i')
      tipos_parametros[0] = 1;

    // tem 2 parâmetros
    if (p2_i_or_a == 'a')
      tipos_parametros[1] = 2;
    else if (p2_i_or_a == 'i')
      tipos_parametros[1] = 1;

    // tem 3 parâmetros
    if (p2_i_or_a == 'a')
      tipos_parametros[2] = 2;
    else if (p2_i_or_a == 'i')
      tipos_parametros[2] = 1;
  }
  else if(r == 5){  //total de 2 parametros
    // tem 1 parametro
    if (p1_i_or_a == 'a')
      tipos_parametros[0] = 2;
    else if (p1_i_or_a == 'i')
      tipos_parametros[0] = 1;

    // tem 2 parâmetros
    if (p2_i_or_a == 'a')
      tipos_parametros[1] = 2;
    else if (p2_i_or_a == 'i')
      tipos_parametros[1] = 1;

  }
  else if(r==3){    // total de 1 parametro
    // tem 1 parametro
    if (p1_i_or_a == 'a')
      tipos_parametros[0] = 2;
    else if (p1_i_or_a == 'i')
      tipos_parametros[0] = 1;
  }

  fclose(f);

  // tradução
  process_function_init(countFunc);
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
  int t, v_p_or_c, i_a_or_r;
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
