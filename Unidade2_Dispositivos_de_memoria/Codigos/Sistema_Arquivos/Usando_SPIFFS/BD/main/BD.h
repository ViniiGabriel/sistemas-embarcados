#ifndef __MEU_BD__
#define __MEU_BD__
#include <inttypes.h>

typedef struct {
    char nomeBD[20];
    uint16_t tamanhoRegistros;
}tipo_configuracao;

typedef struct {
    uint16_t qtd;
    uint16_t tamanhoRegistros;
}cabecalho;


void BD_init (tipo_configuracao config);
void BD_novoRegistro (void *R);
uint16_t BD_quantidade(void);
int BD_pesquisa (int (F)( void *,  void *), void * R);
void BD_mostra (void * buffer, uint16_t posicao);
void BD_remove (int posicaoRegistro);

#endif
