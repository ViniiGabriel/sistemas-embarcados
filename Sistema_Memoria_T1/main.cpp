#include "sa.h"
#include "sa_api.h" // (Ou o cabeçalho onde colocou as suas funções)
#include <stdio.h>
#include <string.h>

// Apenas para garantir que o compilador conhece a função da memória caso não
// esteja no sa_api.h
void memoria_init();
void formata_SA();

int main(void) {
  printf("A iniciar o Sistema de Ficheiros...\n");

  // Nota: Se a sua função inicializa_SA() ainda não existir,
  // chame diretamente a memoria_init() aqui para testar a Camada 2!
  memoria_init();

  // Chama a formatação que acabámos de criar
  formataSA();

  printf(
      "Formatacao concluida! O ficheiro PARTICAO.BIN deve ter sido gerado.\n");

  /* ==========================================================
     COMENTADO TEMPORARIAMENTE PARA TESTAR APENAS A FASE 1
     Descomente isto apenas quando implementar a Camada 4!
     ==========================================================
  char buffer[70], a;
  ARQUIVO *A, *B, *C;

  printf("Tecle [ENTER] para iniciar\r");
  while (1) {
    a = getchar();
    if (a == 13)
      break;
  }
  printf("OK \r");

  A = meu_fopen("UFSC", "w");
  strcpy(buffer, "Oi mundo, teste de mensagemXXX");
  meu_fwrite(A, (uint8_t *)buffer, strlen(buffer));
  meu_fclose(A);

  // ... (restante código do PDF) ...
  */

  return 0;
}
