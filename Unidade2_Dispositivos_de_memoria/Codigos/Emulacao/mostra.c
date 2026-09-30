#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct {
    int qtd;
}Tipo_CABEC;

Tipo_CABEC CABECALHO;

typedef struct {
    char id[20];
    char senha[20];
}Tipo_REG;

FILE *A;
void abre ()
{
    A = fopen ("MEMORIA.BIN","r");
    if (A==NULL)
    {
        printf("Erro, nao existe arquivo\n");
        exit(1);
    }
}
void readBuffer(void *buff, int endInicial, int tam)
{
    fseek(A, endInicial, SEEK_SET);
    fread (buff, (size_t ) tam, 1, A);
}
int mostraCabec(void)
{
    readBuffer(&CABECALHO, 0, sizeof(Tipo_CABEC));
    printf("|------------------------------|\n");
    printf("|  QTD= %4d                   |\n",CABECALHO.qtd);
    return CABECALHO.qtd;
}
void mostraREG(int x)
{
    Tipo_REG R;
    int posicao = sizeof(Tipo_CABEC)+x*sizeof(Tipo_REG);
    readBuffer(&R, posicao, sizeof(Tipo_REG));
    printf("|------------------------------|\n");
    printf("|  ID     %20s |\n",R.id);
    printf("|  SENHA =%20s |\n",R.senha);
    printf("|------------------------------|\n");



}
int main (void)
{
    abre();
    int c = mostraCabec();


    for (int x=0;x<c;x++)
    {
        mostraREG(x);
    }
    return 0;
}
