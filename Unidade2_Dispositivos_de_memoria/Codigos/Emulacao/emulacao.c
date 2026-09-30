#include <stdio.h>
#include <string.h>


typedef struct {
    int qtd;
}Tipo_CABEC;

Tipo_CABEC CABECALHO;

typedef struct {
    char id[20];
    char senha[20];
}Tipo_REG;

FILE *A;


// ********************  Chamadas de baixo nivel *************************
void readBuffer(void *buff, int endInicial, int tam)
{
    fseek(A, endInicial, SEEK_SET);
    fread (buff, (size_t ) tam, 1, A);
}
void writeBuffer(void *buff, int endInicial, int tam)
{
    fseek(A, endInicial, SEEK_SET);
    fwrite (buff, (size_t ) tam, 1, A);
}
// *************************************************************************

// ********************  Chamadas mais alto nivel **************************
void escreveCabec (Tipo_CABEC c)
{
    writeBuffer((void *) &c, 0, sizeof(Tipo_CABEC));
}
int leiaQTD (void)
{
    readBuffer ( (void *) &CABECALHO, 0, sizeof(Tipo_CABEC));
    return CABECALHO.qtd;
}
void incQTD(void)
{
     CABECALHO.qtd++;
     escreveCabec(CABECALHO);
}
void decQTD(void)
{
    CABECALHO.qtd--;
    escreveCabec(CABECALHO);
}

void salvaQTD(void)
{
    escreveCabec(CABECALHO);
}

void insereNovo(char *id, char *senha)
{
    Tipo_REG r;
    int posicao = leiaQTD();
    int enderecoF = posicao* sizeof(Tipo_REG)+sizeof(Tipo_CABEC);
    strcpy(r.id,id);
    strcpy(r.senha, senha);
    writeBuffer(&r, enderecoF, sizeof(Tipo_REG));
    incQTD();
}
void escreveReg(int x, Tipo_REG R)
{
    int posicao = sizeof(Tipo_CABEC)+x*sizeof(Tipo_REG);
    writeBuffer(&R, posicao, sizeof(Tipo_REG));

}

void leiaReg(int x, Tipo_REG *R)
{
    int posicao = sizeof(Tipo_CABEC)+x*sizeof(Tipo_REG);
    readBuffer(R, posicao, sizeof(Tipo_REG));
}

// *************************************************************************


void init ()
{
    CABECALHO.qtd=0;
    A = fopen ("MEMORIA.BIN","r+");
    if (A==NULL)
    {
        A = fopen("MEMORIA.BIN","w+");
        CABECALHO.qtd=0;
    }
    else {
        leiaQTD();
    }
}
void finish(void)
{
    fclose(A);
}
void lista (void)
{
    Tipo_REG tmp;
    int tam = leiaQTD();
    for (int x=0;x<tam;x++)
    {
        leiaReg(x, &tmp);
        printf("ID: %20s  Senha:%10s\n",tmp.id, tmp.senha);
    }
}
void apaga(char *busca)
{
    int tam = leiaQTD();
    Tipo_REG tmp,remover;
    for (int x=0;x<tam;x++)
    {
        leiaReg(x, &tmp);
        if (strcmp(tmp.id,busca)==0)
        {
            leiaReg(tam-1, &remover);
            escreveReg(x, remover);
            decQTD();
            break;
        }
    }
}
void clearBuffer()
{
    char ch;

     while (((ch = getchar()) != EOF) && (ch != '\n')) ;
}
void removeRegistro (void)
{
    char id[20];
    printf("Entre com o ID para remover\n");
    clearBuffer();

    fgets(id, 19, stdin);
    id[strlen(id)-1]=0;
    apaga(id);
}

void add (void)
{
    char id[20];
    char passwd[20];
    printf("Entre com ID:\n");
    clearBuffer();
    fgets(id, 19, stdin);
    id[strlen(id)-1]=0;
    printf("Entre com passwd:\n");

    fgets(passwd, 19, stdin);
    passwd[strlen(passwd)-1]=0;

    insereNovo(id, passwd);

}
void mostraQtd (void)
{
    int tam = leiaQTD();
    printf("Atualmente existem %d registros no BD\n",tam);

}
void inicializaBD (void)
{
    CABECALHO.qtd=0;
    salvaQTD();
}
void recuperaRegistro (void)
{
    char id[20];
    Tipo_REG tmp;
    clearBuffer();
    printf("Entre com o id para buscar:\n");
    fgets(id, 19, stdin);
    id[strlen(id)-1]=0;
    for (int x=0;x<1000;x++)
    {
        printf("%d\n",x);
        leiaReg(x, &tmp);
        if (strcmp(tmp.id,id)==0)
        {
            insereNovo(tmp.id, tmp.senha);

            break;
        }
    }
}
int main (void)
{
    Tipo_CABEC cabec;

    init();
    int opcao;
    while (1)
    {
        printf("[1] Lista todas as IDs e senhas\n");
        printf("[2] Adiciona uma nova entrada (ID e Senha)\n");
        printf("[3] Mostra qtd de pessoas cadastradas\n");
        printf("[4] Remove uma entrada de ID/senha\n");
        printf("[5] Inicializa BD\n");
        printf("[6] Recupera um registro\n");

        opcao = getchar();

        switch (opcao)
        {
            case '1':
                printf("Lista o banco de dados\n");
                lista();
                clearBuffer();

                break;
            case '2':
                add();
                break;
            case '3':
                mostraQtd();
                clearBuffer();

                break;
            case '4':
                removeRegistro();
                break;
            case '5':
                inicializaBD();
                clearBuffer();
                break;
            case '6':
                recuperaRegistro();
                break;
            default:
                printf("opcao invalida\n");
        }
    }


    finish();

    return 0;
}
