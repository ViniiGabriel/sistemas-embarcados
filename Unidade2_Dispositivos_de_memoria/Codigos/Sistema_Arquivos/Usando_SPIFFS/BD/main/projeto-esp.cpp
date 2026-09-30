#include <stdio.h>
#include "esp_spiffs.h"
#include <string.h>
#include "BD.h"

extern "C" void app_main() ;

typedef struct {
    char aluno[20];
    int matricula;
    char curso[30];
    int ano;
}REG;





int comp_matricula ( void * A, void * B)
{
    REG *ptr;
    int * ptr2;
    ptr = (REG *) A;
    ptr2 = (int * )B;
    if (ptr->matricula == *ptr2) return 0;
    return 1;
}

void mostraTela (REG b)
{
   printf("Aluno=%s  Matricula=%d \n",b.aluno, b.matricula);

}

void insereDados(void)
{
    REG dados;

    strcpy(dados.aluno,"Luis Felipe");
    dados.matricula = 456;
    strcpy(dados.curso,"Medicina");
    BD_novoRegistro((void *) &dados);

    strcpy(dados.aluno,"Maria Antonia");
    dados.matricula = 123;
    strcpy(dados.curso,"Eng. Civil");

    BD_novoRegistro((void *) &dados);

    strcpy(dados.aluno,"Pedro terra");
    dados.matricula = 434;
    strcpy(dados.curso,"Matematica");

    BD_novoRegistro((void *) &dados);

    strcpy(dados.aluno,"Ana terra");
    dados.matricula = 556;
    strcpy(dados.curso,"Fisica");

    BD_novoRegistro((void *) &dados);

    strcpy(dados.aluno,"Wolverine");
    dados.matricula = 789;
    strcpy(dados.curso,"Eng. Mecanica");

    BD_novoRegistro((void *) &dados);

}
void app_main(void)
{


    esp_vfs_spiffs_conf_t config = {
        .base_path="/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_vfs_spiffs_register (&config);

    int busca;

    busca = 556;

    tipo_configuracao conf;
    strcpy(conf.nomeBD,"DADOS.DAD");
    conf.tamanhoRegistros = sizeof(REG);

    BD_init (conf);

    insereDados();



    REG S;

    // Antes da remocao
    printf("Banco de dados original\n");
    for (int x=0;x < BD_quantidade();x++)
    {
        BD_mostra ((void *) &S,x);
        mostraTela(S);
    }

    int posicao = BD_pesquisa (comp_matricula, (void *) &busca);

    BD_remove (posicao);

    printf("Depois\n");
    for (int x=0;x < BD_quantidade();x++)
    {
        BD_mostra ((void *) &S,x);
        mostraTela(S);
    }

}
