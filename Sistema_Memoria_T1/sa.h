#ifndef SA_H
#define SA_H

#include <cstdint>
#include <stdint.h>

#define NUM_INODOS 12
#define TAM_BLOCOS 32
#define NUM_BLOCOS 64
#define BLOCO_INVALIDO 0xFFFF

#define ADDR_CABECALHO 0
#define ADDR_INODOS 2
#define ADDR_PONTEIROS (ADDR_INODOS + (NUM_INODOS + sizeof(Inodo)))
#define ADDR_DADOS (ADDR_PONTEIROS + (NUM_BLOCOS * 2))

void formataSA();
uint16_t aloca_bloco();
void desaloca_bloco();
void le_inodo();
void escreve_inodo();

struct Inodo {
  uint8_t status;
  char nome[8];
  uint16_t tamanho;
  uint8_t bloco_direto[32];
  uint16_t ponteiro_indireto;
};

#endif
