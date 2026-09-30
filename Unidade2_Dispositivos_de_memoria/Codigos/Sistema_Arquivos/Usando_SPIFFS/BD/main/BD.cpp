
#include "BD.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

FILE *arq;
uint16_t tamanhoRegistros;
cabecalho cabec;

void leiaCabecalho(cabecalho *cabec)
{
    fseek(arq, 0, SEEK_SET);
    fread(cabec, sizeof(cabecalho), 1, arq);
}
void escrevaCabecalho(cabecalho cabec)
{
    fseek(arq, 0, SEEK_SET);
    fwrite(&cabec, sizeof(cabecalho), 1, arq);
}
void BD_init (tipo_configuracao config)
{
    char buffer[100];

    sprintf(buffer,"/storage/%s",config.nomeBD);
    strcpy(config.nomeBD,buffer);
    arq = fopen (config.nomeBD,"r+");
    if (arq==NULL)
    {
        cabec.qtd = 0;
        cabec.tamanhoRegistros = config.tamanhoRegistros;
        tamanhoRegistros = config.tamanhoRegistros;
        arq = fopen (config.nomeBD,"w+");
        fwrite(&cabec, sizeof(cabec), 1, arq);
    }
    else     leiaCabecalho(&cabec);

}
void BD_novoRegistro (void *R)
{
    uint32_t posicao = (cabec.qtd*cabec.tamanhoRegistros)+sizeof(cabecalho);
    fseek(arq, posicao, SEEK_SET);
    fwrite(R, cabec.tamanhoRegistros, 1, arq);
    cabec.qtd++;
    escrevaCabecalho(cabec);
}
uint16_t BD_quantidade(void)
{
    return cabec.qtd;
}

int BD_pesquisa (int (F)( void *,  void *), void * R)
{
    int retorno;
    void * buffer;
    buffer = malloc (cabec.tamanhoRegistros);
    fseek(arq, sizeof(cabecalho), SEEK_SET);
    for (int x=0;x < cabec.qtd;x++)
    {
        fread(buffer, cabec.tamanhoRegistros, 1, arq);
        retorno = F (buffer, R);
        if (retorno==0)
        {
            free(buffer);
            return x;
        }

    }
    return -1;
}
void leiaRegistro (void *b, uint16_t pos)
{
    fseek(arq, sizeof(cabecalho)+pos*cabec.tamanhoRegistros, SEEK_SET);
    fread(b, cabec.tamanhoRegistros, 1, arq);
}
void escrevaRegistro (void *b, uint16_t pos)
{
    fseek(arq, sizeof(cabecalho)+pos*cabec.tamanhoRegistros, SEEK_SET);
    fwrite(b, cabec.tamanhoRegistros, 1, arq);
}
void BD_remove (int posicaoRegistro)
{
    void * buffer;
    buffer = malloc (cabec.tamanhoRegistros);

    if (posicaoRegistro>=0)
    {
        // Existe item para remover
        leiaRegistro (buffer, cabec.qtd-1);
        escrevaRegistro(buffer,posicaoRegistro );
        cabec.qtd--;
        escrevaCabecalho(cabec);
    }

}


void BD_mostra (void * buffer, uint16_t posicao)
{
    leiaRegistro(buffer, posicao);

}
