#include <stdio.h>
#include "esp_spiffs.h"

void app_main(void)
{


    esp_vfs_spiffs_conf_t config = {
        .base_path="/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_vfs_spiffs_register (&config);
    int contador;

    FILE * f = fopen ("/storage/contador.x", "r+");
    if (f == NULL)
    {   
        printf("O arquivo contador nao existe, criando ele e inicializando...\n");
        f = fopen ("/storage/contador.x","w+");
        contador=0;
    }
    else  fread(&contador, sizeof(contador), 1, f);

    printf("Contador LEGAL vale %d\n",contador);
    contador++;
    fseek(f, 0L, SEEK_SET);
    fwrite(&contador, sizeof(contador), 1, f);
    fclose(f);
}
