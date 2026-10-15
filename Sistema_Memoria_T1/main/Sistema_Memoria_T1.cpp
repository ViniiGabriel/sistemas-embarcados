#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sa_api.h"

extern "C" void app_main(void) {
    char buffer[70];
    ARQUIVO *A, *B, *C;

    // wait for serial monitor to attach
    vTaskDelay(pdMS_TO_TICKS(2000));
    printf("Starting FS initialization\n");

    inicializa_SA();
    formata_SA();

    A = meu_fopen("UFSC", "w");
    strcpy(buffer, "Oi mundo, teste de mensagem XXX");
    meu_fwrite(A, (uint8_t *)buffer, strlen(buffer));
    meu_fclose(A);

    B = meu_fopen("Fabio", "w");
    strcpy(buffer, "Oi mundo, teste de mensagem");
    meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));

    strcpy(buffer, " Prof: FABIO");
    meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));

    strcpy(buffer, " de la Rocha UFSC Ararangua");
    meu_fwrite(B, (uint8_t *)buffer, strlen(buffer));
    meu_fclose(B);

    C = meu_fopen("Fabio", "r");
    meu_fseek(C, 19, SEEK_SET);
    meu_fread(C, (uint8_t *)buffer, 8);
    buffer[8] = 0;

    printf("Lido=%s\n", buffer);
    meu_fclose(C);

    lista_todos();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
