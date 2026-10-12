#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FILE *f;

void memoria_init() {
  printf("Chamou a versao de mem.init do linux\n");

  f = fopen("PARTICAO.BIN", "r+");

  if (f == NULL) {
    printf("Criando um novo arquivo PARTICAO.BIN...\n");
    f = fopen("PARTICAO.BIN", "w+");

    if (f == NULL) {
      printf("Impossivel criar aquivo de particao\n");
      exit(1); // Para o programa se der erro
    }

    uint8_t zero = 0;
    for (int i = 0; i < 32768; i++) {
      fwrite(&zero, 1, 1, f);
    }
    fflush(f);
  }
}

void memoria_fwrite(void *buffer, uint16_t posicao, uint16_t tam) {
  fseek(f, posicao, SEEK_SET);
  fwrite(buffer, tam, 1, f);
  fflush(f);
}

void memoria_fread(void *buffer, uint16_t posicao, uint16_t tam) {
  fseek(f, posicao, SEEK_SET);
  fread(buffer, tam, 1, f);
}
