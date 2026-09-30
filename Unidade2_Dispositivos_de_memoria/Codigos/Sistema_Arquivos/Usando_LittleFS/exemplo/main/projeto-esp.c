#include "esp_littlefs.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static const char *TAG = "LITTLEFS";
#include "driver/uart.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

    char buffer[10000];

static const int RX_BUF_SIZE = 1024;

 FILE *f;

char leiaChar(void)
{

    char c;
    char st=0;

    while (1) {
        
        const int rxBytes = uart_read_bytes(UART_NUM_0, &c, 1, 1000);
        if (c==13) return st;
        st = c;
        vTaskDelay(pdMS_TO_TICKS(200));
        
    }
}

void leiaString (char *st)
{
    int x=0;
    char c;
int rxBytes=1;
    while (1) {
        if (rxBytes==0) vTaskDelay(pdMS_TO_TICKS(200));
        rxBytes = uart_read_bytes(UART_NUM_0, &c, 1, 1000);
        if (rxBytes > 0) {
            if (c==13)
            {
                st[x]=0;
                            return;

            }
            else
            {
               //art_write_bytes(UART_NUM_0,&c, 1 );
                st[x] = c;
                x++;
            }
        }
    }
}
void recebe_arquivo ()
{
    int x=0;
    int rxBytes=1;
    printf("Envie arquivo\n");
    while (1) {        

        if (rxBytes==0) vTaskDelay(pdMS_TO_TICKS(200));
        rxBytes = uart_read_bytes(UART_NUM_0, buffer, 10000, 1000);
        if (rxBytes >0)
        {
            printf("Recebeu %d bytes\n",rxBytes);
            FILE *saida;
            saida = fopen("/littlefs/saida.bin", "w");
            fwrite(buffer, rxBytes, 1, saida);
            fclose(saida);
            printf("Arquivo salvo\n");
            return;
        }
        
    }
}

void mostra_arquivo()
{
     FILE *entrada;

     entrada = fopen("/littlefs/saida.bin", "r");
     if (entrada != NULL)
     {
        int lidos = fread(buffer, 1, sizeof(buffer),  entrada);
        for (int x=0;x<lidos;x++)
        {
            printf("%c",buffer[x]);
        }
        fflush(stdout);
        uart_flush(UART_NUM_0);
        fclose(entrada);
     }
     else printf("Nao existe arquivo \n");
        
}
void littlefs_init(void)
{
    printf("Sou o codigo verao 3.0\n");
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

   f = fopen("/littlefs/lista.txt", "r+");

}
void insere()
{
    char linha[100];
    printf("Entre com o Nome:\n");
    leiaString(linha);
    fseek(f, 0L, SEEK_END);
    fputs(linha,f);
    
    fclose(f);
    f = fopen("/littlefs/lista.txt", "r+");
}
void mostra ()
{

    fseek(f, 0L, SEEK_SET);
    char buffer[100];
    if (f!= NULL)
    {
        while (!feof(f))
        {
            fgets(buffer,99,f);
            printf("%s\n",buffer);
            strcpy(buffer,"");
        }
    }
}

void menu ()
{
    printf("[1] Lista todos\n");
    printf("[2] Insere \n");
    printf("[3] Envia arquivo\n");
    printf("[4] Mostra conteudo arquivo\n");

}
void app_main(void)
{
   uart_config_t uart_config = {
        .baud_rate = 115200,                      //Bluetooth baud rate 
        .data_bits = UART_DATA_8_BITS,          //8 data bits 
        .parity    = UART_PARITY_DISABLE,       //No parity 
        .stop_bits = UART_STOP_BITS_1,          //1 stop bit 
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE   //No hardware flow 
    };
     // se for utilizar a funcao de leitura da uart precisa instalar o driver
     uart_driver_install(UART_NUM_0, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config); 

    littlefs_init();
    char a;
 
menu();
    while (1)
    {
        
        a = leiaChar();
        switch(a)
        {
            case '1':
                mostra();
                 menu();
                break;

            case '2':
                insere();
                menu();        
                break;
            case '3':
                recebe_arquivo();
                menu();
                break;
            case '4':
                mostra_arquivo();
                break;
            default:
                printf("Opcao invalida\n");
                

        }
    }   

}
