#ifndef SA_API_H
#define SA_API_H

#include "sa.h"
#include <cstdint>
#include <stdint.h>
#include <stdio.h>

struct ARQUIVO {
  uint8_t em_uso;   // 1 se este ponteiro ARQUIVO está aberto
  uint8_t id_inodo; // Qual dos 12 inodos (0 a 11) ele representa
  uint16_t
      posicao; // Cursor atual de leitura/escrita (usado pelo fseek e ftell!)
  char modo;   // 'r', 'w', etc.
};

void meu_fopen(const char *nome, const char *modo);
void meu_fwrite(ARQUIVO *arq, uint8_t *buffer, uint16_t tam);
void meu_fread(ARQUIVO *arq, uint8_t *buffer, uint16_t tam);
void meufseek();
void meu_ftell();
void meu_feof();
void meu_fclose();

#endif
