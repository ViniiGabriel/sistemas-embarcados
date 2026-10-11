#ifndef SA_API_H
#define SA_API_H

#include "sa.h"
#include <stdint.h>
#include <stdio.h> // Para SEEK_SET, SEEK_CUR, SEEK_END

// Estrutura do descritor de arquivo aberto (em RAM)
typedef struct {
  uint8_t em_uso;   // Indica se este descritor está aberto (1) ou livre (0)
  uint8_t id_inode; // Índice do inode na tabela (0 a 11)
  uint16_t cursor;  // Posição atual de leitura/escrita (usado em fseek/ftell)
  char modo[3];     // Modo de abertura ("r", "w", "r+")
  Inode
      inode_cache; // Cópia do inode em RAM para evitar leituras desnecessárias
} ARQUIVO;

// Funções gerais do Sistema de Arquivos
void inicializa_SA(void);
void formata_SA(void);
void lista_todos(void);

// Funções de manipulação de arquivos (API)
ARQUIVO *meu_fopen(const char *nome, const char *modo);
uint16_t meu_fwrite(ARQUIVO *arq, uint8_t *buffer, uint16_t tam);
uint16_t meu_fread(ARQUIVO *arq, uint8_t *buffer, uint16_t tam);
int meu_fseek(ARQUIVO *arq, uint16_t offset, int origin);
uint16_t meu_ftell(ARQUIVO *arq);
int meu_feof(ARQUIVO *arq);
int meu_fclose(ARQUIVO *arq);

#endif