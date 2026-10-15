#include "sa_api.h"
#include "memoria.h"
#include "sa.h"
#include <cstdio>
#include <cstring>

ARQUIVO descritores_abertos[4] = {};

static inline uint16_t min_u16(uint16_t a, uint16_t b) {
  return (a < b) ? a : b;
}

void inicializa_SA() {
  memoria_init();
}

void formata_SA() {
  formataSA();
}

static void desaloca_blocos_inodo(Inodo *ino) {
  if (ino == NULL) return;

  if (ino->ponteiro_indireto != BLOCO_INVALIDO) {
    uint16_t ptrs[TAM_BLOCOS / sizeof(uint16_t)];
    memoria_fread(ptrs, ADDR_BLOCO_DADOS(ino->ponteiro_indireto), sizeof(ptrs));

    for (size_t i = 0; i < (TAM_BLOCOS / sizeof(uint16_t)); i++) {
      if (ptrs[i] != BLOCO_INVALIDO) {
        desaloca_bloco(ptrs[i]);
        ptrs[i] = BLOCO_INVALIDO;
      }
    }
    desaloca_bloco(ino->ponteiro_indireto);
    ino->ponteiro_indireto = BLOCO_INVALIDO;
  }
}

ARQUIVO *meu_fopen(const char *nome, const char *modo) {
  if (nome == NULL || modo == NULL) {
    return NULL;
  }

  char modo_c = modo[0];
  if (modo_c != 'r' && modo_c != 'w') {
    printf("Erro: Modo '%s' nao suportado. Use 'r', 'r+' ou 'w'.\n", modo);
    return NULL;
  }

  ARQUIVO *arq = NULL;
  for (int i = 0; i < 4; i++) {
    if (descritores_abertos[i].em_uso == 0) {
      arq = &descritores_abertos[i];
      break;
    }
  }

  if (arq == NULL) {
    printf("Erro: Limite de ficheiros abertos atingido!\n");
    return NULL;
  }

  char nome_pad[8] = {0};
  strncpy(nome_pad, nome, 8);

  Inodo ino;
  int inodo_livre = -1;
  int inodo_encontrado = -1;

  for (int i = 0; i < NUM_INODOS; i++) {
    le_inodo(i, &ino);

    if (ino.status == 0 && inodo_livre == -1) {
      inodo_livre = i;
    }

    if (ino.status == 1 && memcmp(ino.nome, nome_pad, 8) == 0) {
      inodo_encontrado = i;
      break;
    }
  }

  if (modo_c == 'r') {
    if (inodo_encontrado != -1) {
      arq->em_uso = 1;
      arq->id_inodo = (uint8_t)inodo_encontrado;
      arq->posicao = 0;
      arq->modo = (modo[1] == '+') ? '+' : 'r';
      return arq;
    } else {
      printf("Erro: Ficheiro '%s' nao encontrado para leitura.\n", nome);
      return NULL;
    }
  } else if (modo_c == 'w') {
    if (inodo_encontrado != -1) {
      le_inodo(inodo_encontrado, &ino);
      desaloca_blocos_inodo(&ino);
      ino.tamanho = 0;
      ino.ponteiro_indireto = BLOCO_INVALIDO;
      memset(ino.bloco_direto, 0, sizeof(ino.bloco_direto));
      escreve_inodo(inodo_encontrado, &ino);

      arq->em_uso = 1;
      arq->id_inodo = (uint8_t)inodo_encontrado;
      arq->posicao = 0;
      arq->modo = 'w';
      return arq;
    } else {
      if (inodo_livre != -1) {
        memset(&ino, 0, sizeof(Inodo));
        ino.status = 1;
        ino.tamanho = 0;
        ino.ponteiro_indireto = BLOCO_INVALIDO;
        memcpy(ino.nome, nome_pad, 8);

        escreve_inodo(inodo_livre, &ino);

        arq->em_uso = 1;
        arq->id_inodo = (uint8_t)inodo_livre;
        arq->posicao = 0;
        arq->modo = 'w';
        return arq;
      } else {
        printf("Erro: Sem espaco para novos ficheiros (12 inodos ocupados).\n");
        return NULL;
      }
    }
  }

  return NULL;
}

void meu_fclose(ARQUIVO *arq) {
  if (arq != NULL) {
    arq->em_uso = 0;
  }
}

void meu_fwrite(ARQUIVO *arq, uint8_t *buffer, uint16_t tam) {
  if (arq == NULL || arq->em_uso == 0 || buffer == NULL || tam == 0) {
    return;
  }

  if (arq->modo == 'r') {
    printf("Erro: Arquivo aberto em modo somente leitura.\n");
    return;
  }

  Inodo ino;
  le_inodo(arq->id_inodo, &ino);

  uint16_t max_capacidade = TAM_BLOCOS + ((TAM_BLOCOS / sizeof(uint16_t)) * TAM_BLOCOS);
  uint16_t bytes_escritos = 0;

  while (bytes_escritos < tam) {
    uint16_t pos = arq->posicao;
    if (pos >= max_capacidade) {
      printf("Erro: Tamanho maximo do ficheiro atingido (%u bytes).\n", max_capacidade);
      break;
    }

    if (pos < TAM_BLOCOS) {
      uint16_t espaco_bloco = TAM_BLOCOS - pos;
      uint16_t chunk = min_u16(tam - bytes_escritos, espaco_bloco);

      memcpy(&ino.bloco_direto[pos], buffer + bytes_escritos, chunk);
      bytes_escritos += chunk;
      arq->posicao += chunk;

      if (arq->posicao > ino.tamanho) {
        ino.tamanho = arq->posicao;
      }
      escreve_inodo(arq->id_inodo, &ino);
    } else {
      uint16_t offset_ind = pos - TAM_BLOCOS;
      uint16_t idx_ptr = offset_ind / TAM_BLOCOS;
      uint16_t offset_bloco = offset_ind % TAM_BLOCOS;

      if (ino.ponteiro_indireto == BLOCO_INVALIDO) {
        uint16_t novo_bloco_ind = aloca_bloco();
        if (novo_bloco_ind == BLOCO_INVALIDO) {
          printf("Erro: Nao ha blocos livres para indirecao!\n");
          break;
        }

        uint16_t tabela_ptrs[TAM_BLOCOS / sizeof(uint16_t)];
        for (size_t i = 0; i < (TAM_BLOCOS / sizeof(uint16_t)); i++) {
          tabela_ptrs[i] = BLOCO_INVALIDO;
        }
        memoria_fwrite(tabela_ptrs, ADDR_BLOCO_DADOS(novo_bloco_ind), sizeof(tabela_ptrs));

        ino.ponteiro_indireto = novo_bloco_ind;
        escreve_inodo(arq->id_inodo, &ino);
      }

      uint16_t bloco_dados;
      uint16_t endereco_ptr = ADDR_BLOCO_DADOS(ino.ponteiro_indireto) + (idx_ptr * sizeof(uint16_t));
      memoria_fread(&bloco_dados, endereco_ptr, sizeof(uint16_t));

      if (bloco_dados == BLOCO_INVALIDO) {
        bloco_dados = aloca_bloco();
        if (bloco_dados == BLOCO_INVALIDO) {
          printf("Erro: Nao ha blocos livres para dados!\n");
          break;
        }

        uint8_t bloco_zeros[TAM_BLOCOS] = {0};
        memoria_fwrite(bloco_zeros, ADDR_BLOCO_DADOS(bloco_dados), TAM_BLOCOS);
        memoria_fwrite(&bloco_dados, endereco_ptr, sizeof(uint16_t));
      }

      uint16_t espaco_bloco = TAM_BLOCOS - offset_bloco;
      uint16_t chunk = min_u16(tam - bytes_escritos, espaco_bloco);

      memoria_fwrite(buffer + bytes_escritos, ADDR_BLOCO_DADOS(bloco_dados) + offset_bloco, chunk);
      bytes_escritos += chunk;
      arq->posicao += chunk;

      if (arq->posicao > ino.tamanho) {
        ino.tamanho = arq->posicao;
        escreve_inodo(arq->id_inodo, &ino);
      }
    }
  }
}

void meu_fread(ARQUIVO *arq, uint8_t *buffer, uint16_t tam) {
  if (arq == NULL || arq->em_uso == 0 || buffer == NULL || tam == 0) {
    return;
  }

  Inodo ino;
  le_inodo(arq->id_inodo, &ino);

  if (arq->posicao >= ino.tamanho) {
    return;
  }

  uint16_t disp_total = ino.tamanho - arq->posicao;
  uint16_t total_a_ler = min_u16(tam, disp_total);
  uint16_t bytes_lidos = 0;

  while (bytes_lidos < total_a_ler) {
    uint16_t pos = arq->posicao;

    if (pos < TAM_BLOCOS) {
      uint16_t espaco_bloco = TAM_BLOCOS - pos;
      uint16_t chunk = min_u16(total_a_ler - bytes_lidos, espaco_bloco);

      memcpy(buffer + bytes_lidos, &ino.bloco_direto[pos], chunk);
      bytes_lidos += chunk;
      arq->posicao += chunk;
    } else {
      uint16_t offset_ind = pos - TAM_BLOCOS;
      uint16_t idx_ptr = offset_ind / TAM_BLOCOS;
      uint16_t offset_bloco = offset_ind % TAM_BLOCOS;

      if (ino.ponteiro_indireto == BLOCO_INVALIDO) {
        break;
      }

      uint16_t bloco_dados;
      uint16_t endereco_ptr = ADDR_BLOCO_DADOS(ino.ponteiro_indireto) + (idx_ptr * sizeof(uint16_t));
      memoria_fread(&bloco_dados, endereco_ptr, sizeof(uint16_t));

      if (bloco_dados == BLOCO_INVALIDO) {
        break;
      }

      uint16_t espaco_bloco = TAM_BLOCOS - offset_bloco;
      uint16_t chunk = min_u16(total_a_ler - bytes_lidos, espaco_bloco);

      memoria_fread(buffer + bytes_lidos, ADDR_BLOCO_DADOS(bloco_dados) + offset_bloco, chunk);
      bytes_lidos += chunk;
      arq->posicao += chunk;
    }
  }
}

int meu_fseek(ARQUIVO *arq, int32_t offset, int whence) {
  if (arq == NULL || arq->em_uso == 0) {
    return -1;
  }

  Inodo ino;
  le_inodo(arq->id_inodo, &ino);

  int32_t nova_pos;
  switch (whence) {
    case SEEK_SET:
      nova_pos = offset;
      break;
    case SEEK_CUR:
      nova_pos = (int32_t)arq->posicao + offset;
      break;
    case SEEK_END:
      nova_pos = (int32_t)ino.tamanho + offset;
      break;
    default:
      return -1;
  }

  if (nova_pos < 0) {
    return -1;
  }

  arq->posicao = (uint16_t)nova_pos;
  return 0;
}

uint16_t meu_ftell(ARQUIVO *arq) {
  if (arq == NULL || arq->em_uso == 0) {
    return 0;
  }
  return arq->posicao;
}

int meu_feof(ARQUIVO *arq) {
  if (arq == NULL || arq->em_uso == 0) {
    return 1;
  }
  Inodo ino;
  le_inodo(arq->id_inodo, &ino);
  return (arq->posicao >= ino.tamanho) ? 1 : 0;
}

void lista_todos() {
  printf("\n--- Arquivos no Sistema ---\n");
  int count = 0;
  for (int i = 0; i < NUM_INODOS; i++) {
    Inodo ino;
    le_inodo(i, &ino);
    if (ino.status == 1) {
      char nome[9] = {0};
      memcpy(nome, ino.nome, 8);
      printf("[%2d] Nome: %-8s | Tam: %4u B | Indireto: ", i, nome, ino.tamanho);
      if (ino.ponteiro_indireto == BLOCO_INVALIDO) {
        printf("NENHUM\n");
      } else {
        printf("Bloco %u\n", ino.ponteiro_indireto);
      }
      count++;
    }
  }
  if (count == 0) {
    printf("(Nenhum arquivo gravado)\n");
  }
  printf("---------------------------\n\n");
}
