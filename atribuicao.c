#include <stdio.h>
#include <string.h>
#include "atribuicao.h"

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