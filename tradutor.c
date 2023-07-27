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

int main()
{
  char p1, p2, p3;
  int r, i1, i2, i3;
  int inicio, fim, passo;
  char line[LINESZ];
  int count = 0;

  char tipo[4];
  char v_i_or_a, p1_i_or_a, p2_i_or_a, p3_i_or_a;
  int size_vet;
  int countVar = 0;

  int countFunc = 0;

  int tam_pilha, aux;

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
        r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &countFunc, &p1_i_or_a, &p1, &p2_i_or_a, &p2, &p3_i_or_a, &p3);
        switch (r) {
            // não tem parametros
            case 1:
              break;

            // tem 1 parâmetro
            case 3:
              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
              break;

            // tem 2 parâmetros
            case 5:
              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p1_i_or_a, p1, -tam_pilha);
              tam_pilha+=8;
              fprintf(fb, "\t# p%c%d -> %d(%%rbp)\n", p2_i_or_a, p2, -tam_pilha);
              break;

            // tem 3 parâmetros  
            case 7:
              tam_pilha+=8;
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

    sscanf(line, "%s v%c%d size ci%d", tipo, v_i_or_a, &countVar, &size_vet);

    if (strncmp(tipo, "var", 3) == 0) {
      v_i_or_a = 'i';
      tam_pilha += 4;
      fprintf(fb, "\t# v%c%d -> %d(%%rbp)\n", v_i_or_a, countVar, -tam_pilha);
      continue;
    }

    else if(strcmp(tipo, "reg") == 0){
      v_i_or_a = 'i';
      tam_pilha += 4;
      fprintf(fb, "\t# r%c%d -> %d(%%rbp)\n", v_i_or_a, countVar, -tam_pilha);
      continue;
    }

    else if(strcmp(tipo, "vet") == 0){
      v_i_or_a = 'a';
      (tam_pilha) += 4 * size_vet;
      aux = -tam_pilha;
      for(int i = 0; i < size_vet; i++, aux += 4){
        fprintf(fb, "\t# v%c%d[%d] -> %d(%%rbp)\n", v_i_or_a, countVar, i, aux);
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
    
    // -------------------------- CONDICIONAL ------------------

    // Verifica se é um 'if'
    r = sscanf(line, "if v%d > v%d", &i1, &i2);
    if (r == 2) {
      printf("Linha %d: %s\n", count, line);
      printf("Indices: %d e %d\n", i1, i2);
      printf("---\n");
      continue;
    }
  }

  return 0;
}
