#ifndef SA_API_H
#define SA_API_H

#include <cstdint>
#include <stdint.h>

struct ARQUIVO {
  uint8_t em_uso;
  uint8_t id_inodo;
  uint16_t posicao;
  char modo;
};

extern ARQUIVO descritores_abertos[4];

// Camada 4 - API do Sistema de Arquivos
void inicializa_SA();
void formata_SA();

ARQUIVO *meu_fopen(const char *nome, const char *modo);
void meu_fwrite(ARQUIVO *arq, uint8_t *buffer, uint16_t tam);
void meu_fread(ARQUIVO *arq, uint8_t *buffer, uint16_t tam);
int meu_fseek(ARQUIVO *arq, int32_t offset, int whence);
uint16_t meu_ftell(ARQUIVO *arq);
int meu_feof(ARQUIVO *arq);
void meu_fclose(ARQUIVO *arq);

// Camada de Aplicacao
void lista_todos();

#endif
