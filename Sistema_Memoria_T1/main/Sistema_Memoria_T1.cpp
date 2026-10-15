#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sa_api.h"
#include "sa.h"

// Funcao para formatar e gravar os arquivos de teste
static void escrever_arquivos_teste(void) {
    char buffer[70];
    ARQUIVO *A, *B;

    printf("\n--- Gravando Arquivos de Teste ---\n");

    printf("Gravando arquivo \"UFSC\"...\n");
    A = meu_fopen("UFSC", "w");
    if (A != NULL) {
        strcpy(buffer, "Oi mundo, teste de mensagem XXX");
        meu_fwrite(A, (uint8_t *)buffer, strlen(buffer));
        meu_fclose(A);
        printf(" -> Arquivo \"UFSC\" gravado com sucesso! (%d bytes)\n", (int)strlen(buffer));
    } else {
        printf(" -> Erro ao abrir arquivo \"UFSC\" para escrita.\n");
    }

    printf("Gravando arquivo \"Fabio\"...\n");
    B = meu_fopen("Fabio", "w");
    if (B != NULL) {
        strcpy(buffer, "Oi mundo, teste de mensagem");
        meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));

        strcpy(buffer, " Prof: FABIO");
        meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));

        strcpy(buffer, " de la Rocha UFSC Ararangua");
        meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));
        meu_fclose(B);
        printf(" -> Arquivo \"Fabio\" gravado com sucesso!\n");
    } else {
        printf(" -> Erro ao abrir arquivo \"Fabio\" para escrita.\n");
    }

    printf("Gravacao concluida!\n");
    printf("----------------------------------\n");
}

// Funcao para imprimir o conteudo dos arquivos em formato hexadecimal
static void imprimir_hex_arquivos(void) {
    printf("\n====================================================================\n");
    printf("                  CONTEUDO HEX DOS ARQUIVOS SALVOS                  \n");
    printf("====================================================================\n");

    int total_arquivos = 0;

    for (int i = 0; i < NUM_INODOS; i++) {
        Inodo ino;
        le_inodo(i, &ino);

        if (ino.status == 1) {
            total_arquivos++;
            char nome[9] = {0};
            memcpy(nome, ino.nome, 8);

            printf("\n>> Arquivo [%d]: \"%s\" | Tamanho: %u bytes\n", i, nome, ino.tamanho);
            if (ino.ponteiro_indireto == BLOCO_INVALIDO) {
                printf("   Indirecao: NENHUMA\n");
            } else {
                printf("   Indirecao: Bloco de ponteiros %u\n", ino.ponteiro_indireto);
            }

            if (ino.tamanho == 0) {
                printf("   (Arquivo com tamanho 0 bytes)\n");
                continue;
            }

            ARQUIVO *arq = meu_fopen(nome, "r");
            if (arq == NULL) {
                printf("   Erro ao abrir arquivo \"%s\" para leitura.\n", nome);
                continue;
            }

            printf("\n   Tabela Hexadecimal (Hexdump):\n");
            printf("   Offset  | 00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F | ASCII\n");
            printf("   --------+-------------------------------------------------+-----------------\n");

            uint16_t offset = 0;
            while (offset < ino.tamanho) {
                uint8_t linha_buf[16];
                uint16_t a_ler = (ino.tamanho - offset > 16) ? 16 : (ino.tamanho - offset);

                meu_fread(arq, linha_buf, a_ler);

                printf("   0x%04X  | ", offset);

                for (uint16_t j = 0; j < 16; j++) {
                    if (j < a_ler) {
                        printf("%02X ", linha_buf[j]);
                    } else {
                        printf("   ");
                    }
                    if (j == 7) {
                        printf(" ");
                    }
                }

                printf("| ");
                for (uint16_t j = 0; j < a_ler; j++) {
                    uint8_t c = linha_buf[j];
                    if (c >= 32 && c <= 126) {
                        printf("%c", c);
                    } else {
                        printf(".");
                    }
                }
                printf("\n");

                offset += a_ler;
            }
            printf("   --------+-------------------------------------------------+-----------------\n");

            // Imprime também os bytes hexadecimais brutos em sequencia
            printf("   Bytes HEX brutos:\n   ");
            meu_fseek(arq, 0, SEEK_SET);
            uint8_t byte_lido;
            for (uint16_t k = 0; k < ino.tamanho; k++) {
                meu_fread(arq, &byte_lido, 1);
                printf("%02X ", byte_lido);
                if ((k + 1) % 16 == 0 && (k + 1) < ino.tamanho) {
                    printf("\n   ");
                }
            }
            printf("\n");

            meu_fclose(arq);
        }
    }

    if (total_arquivos == 0) {
        printf("\nNenhum arquivo gravado no sistema de arquivos.\n");
        printf("Dica: Use a opcao 3 para gravar os arquivos de teste.\n");
    }

    printf("====================================================================\n\n");
}

// Funcao de teste de leitura usando seek (arquivo Fabio)
static void testar_leitura_seek(void) {
    printf("\n--- Teste de Leitura com fseek ---\n");
    ARQUIVO *C = meu_fopen("Fabio", "r");
    if (C == NULL) {
        printf("Arquivo \"Fabio\" nao encontrado para teste.\n");
        return;
    }

    char buffer[16] = {0};
    meu_fseek(C, 19, SEEK_SET);
    meu_fread(C, (uint8_t *)buffer, 8);
    buffer[8] = '\0';

    printf("Posicao 19, lendo 8 bytes do arquivo \"Fabio\":\n");
    printf("Lido = \"%s\"\n", buffer);
    meu_fclose(C);
    printf("----------------------------------\n");
}

// Exibe o menu interativo
static void exibir_menu(void) {
    printf("\n==================================================\n");
    printf("             MENU SISTEMA DE ARQUIVOS             \n");
    printf("==================================================\n");
    printf(" 1 - Imprimir conteudo HEX dos arquivos salvos    \n");
    printf(" 2 - Formatar a memoria                           \n");
    printf(" 3 - Escrever arquivos novamente                  \n");
    printf(" 4 - Listar arquivos (metadados)                  \n");
    printf(" 5 - Testar leitura com seek (arquivo \"Fabio\")    \n");
    printf("==================================================\n");
    printf("Escolha uma opcao: ");
    fflush(stdout);
}

// Leitura de caractere pela porta serial (ignora quebras de linha residuais)
static char ler_opcao_serial(void) {
    while (1) {
        int c = getchar();
        if (c != EOF && c > 0) {
            if (c == '\r' || c == '\n') {
                continue;
            }
            return (char)c;
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

extern "C" void app_main(void) {
    // Aguarda o monitor serial anexar
    vTaskDelay(pdMS_TO_TICKS(2000));
    printf("\nIniciando Sistema de Arquivos...\n");

    inicializa_SA();

    // Inicializacao com formatacao e escrita inicial
    formata_SA();
    escrever_arquivos_teste();

    // Teste de leitura com seek inicial (como no original)
    testar_leitura_seek();

    // Lista metadados e imprime os conteudos hexadecimais inicialmente
    lista_todos();
    imprimir_hex_arquivos();

    // Loop do Menu Interativo via Serial
    while (1) {
        exibir_menu();
        char opcao = ler_opcao_serial();
        printf("%c\n", opcao);

        switch (opcao) {
            case '1':
                imprimir_hex_arquivos();
                break;
            case '2':
                printf("\nFormatando a memoria...\n");
                formata_SA();
                printf("Memoria formatada com sucesso!\n");
                lista_todos();
                break;
            case '3':
                escrever_arquivos_teste();
                lista_todos();
                break;
            case '4':
                lista_todos();
                break;
            case '5':
                testar_leitura_seek();
                break;
            default:
                printf("\nOpcao '%c' invalida! Por favor, escolha uma opcao de 1 a 5.\n", opcao);
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
