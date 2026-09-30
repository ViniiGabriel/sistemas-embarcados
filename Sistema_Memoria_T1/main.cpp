#include "sa_api.h"
#include <stdio.h>
#include <string.h>

int main(void) {
  char buffer[70], a;
  ARQUIVO *A, *B, *C;

  printf("Tecle [ENTER] para iniciar\r");
  while (1) {
    a = getchar();
    if (a == 13)
      break;
  }
  printf("OK \r");

  inicializa_SA();
  formata_SA(); // realizado apenas 1 vez

  A = meu_fopen("UFSC", "w");
  strcpy(buffer, "Oi mundo, teste de mensagemXXX");
  meu_fwrite(A, (uint8_t *)buffer, strlen(buffer));
  meu_fclose(A);

  B = meu_fopen("Fabio", "w");
  strcpy(buffer, "Oi mundo, teste de mensagemXXX");
  meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));

  strcpy(buffer, " Prof: FABIO");
  meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));

  strcpy(buffer, " de la Rocha UFSC Ararangua");
  meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));
  meu_fclose(B);

  C = meu_fopen("Fabio", "r");
  meu_fseek(C, 19, SEEK_SET);
  meu_fread(C, (uint8_t *)buffer, 8);
  buffer[8] = 0;
  printf("Lido=%s\r", buffer);

  lista_todos();

  while (1) {
  }
  return 0;
}
