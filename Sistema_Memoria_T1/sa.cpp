#include "sa.h"
#include "memoria.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <sys/types.h>

void formataSA() {
  uint16_t primeiro_livre = 0;
  memoria_fwrite(&primeiro_livre, 0, sizeof(uint16_t));

  Inodo inodo_vazio;
  memset(&inodo_vazio, 0, sizeof(Inodo));

  for (size_t i = 0; i < 12; i++) {
    uint16_t endereco_inodo = 2 + (i * sizeof(Inodo));

    memoria_fwrite(&inodo_vazio, endereco_inodo, sizeof(Inodo));
  }

  for (size_t i = 0; i < NUM_BLOCOS; i++) {
    uint16_t prox_bloco;
    uint16_t endereco_base = 2 + (12 * sizeof(Inodo));

    if (i == NUM_BLOCOS - 1) {
      prox_bloco = 0xFFFF;
    } else {
      prox_bloco = i + 1;
    }

    uint16_t endereco_escrita = endereco_base + (i * 2);

    memoria_fwrite(&prox_bloco, endereco_escrita, sizeof(uint16_t));
  }
}

uint16_t aloca_bloco() {
  uint16_t bloco_alocado;
  uint16_t prox_livre;

  memoria_fread(&bloco_alocado, ADDR_CABECALHO, sizeof(uint16_t));

  if (bloco_alocado == BLOCO_INVALIDO) {
    printf("Erro: Disco cheio");
    return BLOCO_INVALIDO;
  }

  uint16_t endereco_do_ponteiro = ADDR_PONTEIROS + (bloco_alocado * 2);

  memoria_fread(&prox_livre, endereco_do_ponteiro, sizeof(uint16_t));

  memoria_fwrite(&prox_livre, ADDR_CABECALHO, sizeof(uint16_t));

  return bloco_alocado;
}

void desaloca_bloco(uint16_t bloco) {
  uint16_t primeiro_livre;

  memoria_fread(&primeiro_livre, ADDR_CABECALHO, sizeof(uint16_t));

  uint16_t endereco_do_ponteiro = ADDR_PONTEIROS + (bloco * 2);
  memoria_fwrite(&primeiro_livre, endereco_do_ponteiro, sizeof(uint16_t));

  memoria_fwrite(&bloco, ADDR_CABECALHO, sizeof(uint16_t));
}

void escreve_inodo(int id, Inodo *inodo) {
  if (id < 0 || id >= 12) {
    return;
  }

  uint16_t endereco_escrita = ADDR_INODOS + (id * sizeof(Inodo));

  memoria_fwrite(inodo, endereco_escrita, sizeof(Inodo));
}

void le_inodo(int id, Inodo *inodo) {
  if (id < 0 || id >= 12) {
    return;
  }

  uint16_t endereco_leitura = ADDR_INODOS + (id * sizeof(Inodo));

  memoria_fread(inodo, endereco_leitura, sizeof(Inodo));
}
