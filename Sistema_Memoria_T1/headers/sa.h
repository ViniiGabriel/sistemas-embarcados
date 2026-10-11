#ifndef SA_H
#define SA_H

#include <stdint.h>

#define MAX_ARQUIVOS 12
#define TAM_NOME 8
#define TAM_BLOCO 32
#define NUM_BLOCOS 64
#define PONTEIROS_POR_BLOCO 16
#define BLOCO_INVALIDO 0xFFFF

#define OFFSET_CABECALHO 0
#define TAM_INODE 45
#define OFFSET_INODES 2
#define OFFSET_PONTEIROS (OFFSET_INODES + (MAX_ARQUIVOS * TAM_INODE)) // 542
#define OFFSET_DADOS (OFFSET_PONTEIROS + (NUM_BLOCOS * 2)) // 542 + 2*N

typedef struct __attribute__((packed)) {
  uint8_t status;                  // 0 = livre, 1 = ocupado (1 byte)
  char nome[TAM_NOME];             // Nome do arquivo (8 bytes)
  uint16_t tamanho;                // Tamanho em bytes (2 bytes)
  uint8_t bloco_direto[TAM_BLOCO]; // Dados diretos (32 bytes)
  uint16_t
      ponteiro_indireto; // Índice do bloco de ponteiros indiretos (2 bytes)
} Inode;

// Funções auxiliares de cálculo de endereço físico
static inline uint16_t endereco_inode(uint8_t id_inode) {
  return OFFSET_INODES + (id_inode * TAM_INODE);
}

static inline uint16_t endereco_ponteiro(uint16_t id_bloco) {
  return OFFSET_PONTEIROS + (id_bloco * 2);
}

static inline uint16_t endereco_bloco_dados(uint16_t id_bloco) {
  return OFFSET_DADOS + (id_bloco * TAM_BLOCO);
}

// Protótipos das funções internas (Camada 3 - sa.cpp)
void sa_ler_inode(uint8_t id_inode, Inode *inode);
void sa_escrever_inode(uint8_t id_inode, const Inode *inode);
uint16_t sa_alocar_bloco(void);
void sa_desalocar_bloco(uint16_t id_bloco);
void sa_liberar_blocos_arquivo(Inode *inode);

#endif
