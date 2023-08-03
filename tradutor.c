#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256

// Remove o '\n' do fim da linha
void remove_newline(char *ptr)
{
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}

void alinha_pilha(int t_pilha, int alinhamento){
  // ve se ta alinhado
    if(t_pilha < alinhamento) t_pilha+=(alinhamento-t_pilha);
    t_pilha+=(t_pilha%alinhamento);
}

int main()
{
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
  int countVar = 0;

  int countFunc = 0;

  int tam_pilha, alinhamento, aux;

  FILE *fb = fopen("file.S", "a+");

  // Lê uma linha por vez
  while (fgets(line, LINESZ, stdin) != NULL) {
    count++;
    remove_newline(line);

    // Verifica se line começa com 'end' (3 letras)
    if (strncmp(line, "end", 3) == 0) {
      //retorna os registradores ?????????????????????????????


      fprintf(fb, "\tleave\n\tret\n");	      
      continue;
    }

    // ---------------------- DEFINIÇÃO DE FUNÇÃO -------------------
    // verifica se a linha começa com 'function'
      if (strncmp(line, "function", 8) == 0) { 
        tam_pilha = 0;
        alinhamento = 4;
        r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &countFunc, &p1_i_or_a, &p1, &p2_i_or_a, &p2, &p3_i_or_a, &p3);
        switch (r) {
            // não tem parametros
            case 1:
              break;

            // tem 1 parâmetro
            case 3:
              if(p1_i_or_a == 'a'){
                tam_pilha+=8;
                alinhamento = 8;
              }
              else if(p1_i_or_a == 'i') tam_pilha+=4;
              alinha_pilha(tam_pilha, alinhamento);
              // NÃO SEI COMO ALINHAR ISSSOOOOOOOOOOOOOOO!!!!!!!!!

              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
              break;

            // tem 2 parâmetros
            case 5:
              // ...

              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p2_i_or_a, p2, -tam_pilha);
              break;

            // tem 3 parâmetros  
            case 7:
              if(p1_i_or_a == 'a')tam_pilha+=8;
              else if(p1_i_or_a == 'i') tam_pilha+=4;
              //...
              
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p2_i_or_a, p2, -tam_pilha);
              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p3_i_or_a, p3, -tam_pilha);    
              break;
        }
        continue;
      }


    // ------------- DEFINIÇÃO DE VARIÁVEIS LOCAIS --------------



    // verifica se a linha começa com 'def'
    if (strncmp(line, "def", 3) == 0) {
        fprintf(fb, "\t# Definindo variáveis locais\n");	      
        continue;
    }

    // declara vetor
    sscanf(line, "%s v%c%d size ci%d", &tipo, &i_a_or_r, &countVar, &size_vet);

    if (strncmp(tipo, "var", 3) == 0) {
      i_a_or_r = 'i';
      tam_pilha += 4;
      fprintf(fb, "\t# v%c%d -> %d(%%rbp)\n", i_a_or_r, countVar, -tam_pilha);
      continue;
    }

    // declara registrador
    else if(strcmp(tipo, "reg") == 0){
      i_a_or_r = 'r';
      tam_pilha += 4;
      fprintf(fb, "\t# r%c%d -> %d(%%rbp)\n", i_a_or_r, countVar, -tam_pilha);
      continue;
    }

    else if(strcmp(tipo, "vet") == 0){
      i_a_or_r = 'a';
      (tam_pilha) += 4 * size_vet;
      aux = -tam_pilha;
      for(int i = 0; i < size_vet; i++, aux += 4){
        fprintf(fb, "\t# v%c%d[%d] -> %d(%%rbp)\n", i_a_or_r, countVar, i, aux);
      }
      continue;
    }

    // verifica se a linha começa com 'enddef'
    if (strncmp(line, "enddef", 6) == 0) {
        // ve se ta alinhado
        if(tam_pilha < 16) tam_pilha+=(16-tam_pilha);
        tam_pilha+=tam_pilha%16;

        fprintf(fb, "\tsubq $%d, %%rsp\n", tam_pilha);
	      
        continue;
    }

    // -------------------------- ATRIBUIÇÃO ------------------

    // -------------------------- CHAMADA FUNÇÃO ------------------

    // -------------------------- ACESSO ARRAY ------------------
    
    // -------------------------- CONDICIONAL ------------------

    // Verifica se é um 'if'
    r = sscanf(line, "if %ci%d %s %ci%d", &v_p_or_c, &co1, &cond, &v_p_or_c, &co2);
    if (r) {
      countIf++;
      // continua??????????????????????????????????????????????????????????????
      fprintf(fb, "\tif%d:\n\t...\n", countIf);
      continue;
    }

    // -------------------------- RETORNO ------------------

    // Verifica se é um 'return'
    r = sscanf(line, "return %ci%d", &v_p_or_c, &r1);
    if(r){

      // retorna uma variável
      if(strncmp(v_p_or_c, 'v', 1) == 0)
        // onde essa variavel ta na pilha???????????????????????????????????????????
        fprintf(fb, "\tmovl ?(%%rbp), %%eax\n", r1);

      // retorna um parâmetro
      else if(strncmp(v_p_or_c, 'p', 1) == 0){
        // se for assim, considera que os parâmetros
        // são salvos na pilha primeiro
        if(r1 == 1) fprintf(fb, "\tmovl -8(%%rbp), %%eax\n");
        // ou
        // if(r1 == 1) fprintf(fb, "\tmovl %%edi, %%eax\n");
        // ???????????????????????????????????????????????????????
        else if(r1 == 2) fprintf(fb, "\tmovl -16(%%rbp), %%eax\n");
        else if(r1 == 3) fprintf(fb, "\tmovl -24(%%rbp), %%eax\n");
      }

      // retorna uma constante
      else if(strncmp(v_p_or_c, 'c', 1) == 0)
        fprintf(fb, "\tmovl $%d, %%eax\n", r1);
      continue;
    }
  }

  return 0;
}
