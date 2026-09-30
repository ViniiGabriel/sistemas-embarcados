#include "esp_littlefs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static const char *TAG = "LITTLEFS";

void littlefs_init(void)
{
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao montar LittleFS (%s)", esp_err_to_name(ret));
        return;
    }

    size_t total = 0, used = 0;
    esp_littlefs_info("littlefs", &total, &used);

    ESP_LOGI(TAG, "LittleFS montado");
    ESP_LOGI(TAG, "Total: %d", total);
    ESP_LOGI(TAG, "Usado: %d", used);

    FILE *f = fopen("/littlefs/style.css", "r");
    printf("ESSA EH A VERSAO 3.0\n\n");
    char buffer[100];
    if (f!= NULL)
    {
        while (!feof(f))
        {
            fgets(buffer,99,f);
            printf("%s\n",buffer);
        }
    }

}


void app_main(void)
{
    littlefs_init();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
