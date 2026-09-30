#include <stdio.h>
#include "esp_spiffs.h"

void app_main(void)
{
    int contador=1;

    esp_vfs_spiffs_conf_t config = {
        .base_path="/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_vfs_spiffs_register (&config);

    FILE * f = fopen ("/storage/arquivo.bin", "r+");
    if (f == NULL)
    {
        // nao existe, entao cria;
        f = fopen ("/storage/arquivo.bin", "w");
        fwrite(&contador, sizeof(contador), 1, f);
        printf("Contador vale:0\n");      
        return;
    }
    else
    {
        fread (&contador, sizeof(contador), 1, f);
        printf("Contador vale:%d\n",contador);
        contador++;
        fseek(f, 0L, SEEK_SET);
        fwrite(&contador, sizeof(contador), 1, f);

    }
    fclose(f);
}