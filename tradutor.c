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
  char v1, v2, v3;
  int r, i1, i2, i3;
  int inicio, fim, passo;
  char line[LINESZ];
  int count = 0;

  char tipo[4];
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
      //retorna os registradores


      printf("leave\nret\n");	      
      continue;
    }

    // ---------------------- DEFINIÇÃO DE FUNÇÃO -------------------
    // verifica se a linha começa com 'function'
      if (strncmp(line, "function", 8) == 0) { 
        tam_pilha = 0;
        r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &countFunc, &v1, &i1, &v2, &i2, &v3, &i3);
        switch (r) {
            // não tem parametros
            case 1:
              break;

            // tem 1 parâmetro
            case 3:
              break;

            // tem 2 parâmetros
            case 5:
              break;

            // tem 3 parâmetros  
            case 7:
               printf("Parâmetros da função f%d: %c%d, %c%d, %c%d\n", countFunc, v1, i1, v2, i2, v3, i3);
               printf("---\n");
               break;
        }
        continue;
      }


    // ------------- DEFINIÇÃO DE VARIÁVEIS LOCAIS --------------

    // verifica se a linha começa com 'def'
    if (strncmp(line, "def", 3) == 0) {
        printf("Linha %d: %s\n", count, line);
        printf("Definição escopo de função: {\n");
        printf("---\n");	      
        continue;
    }

    sscanf(line, "%s vi%d size ci%d", tipo, &countVar, &size_vet);

    if (strncmp(tipo, "var", 3) == 0) {
      tam_pilha += 4;
      fprintf(fb, "\t# vi%d -> %d(%%rbp)\n", countVar, -tam_pilha);
      continue;
    }

    else if(strcmp(tipo, "reg") == 0){
      tam_pilha += 4;
      fprintf(fb, "\t# ri%d -> %d(%%rbp)\n", countVar, -tam_pilha);
      continue;
    }

    else if(strcmp(tipo, "vet") == 0){
      (tam_pilha) += 4 * size_vet;
      aux = -tam_pilha;
      for(int i = 0; i < size_vet; i++, aux += 4){
        fprintf(fb, "\t# va%d[%d] -> %d(%%rbp)\n", countVar, i, aux);
      }
      continue;
    }

    // verifica se a linha começa com 'enddef'
    if (strncmp(line, "enddef", 6) == 0) {
        printf("}\nLinha %d: %s\n", count, line);
        printf("Fim da definição de escopo:\n");
        printf("---\n");	      
        continue;
    }
    
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
