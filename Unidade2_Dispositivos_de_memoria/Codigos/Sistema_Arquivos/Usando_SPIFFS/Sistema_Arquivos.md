| <img src="https://arquivos.ufsc.br/d/8d497afc6d9e4d3c9c25/files/?p=%2Fufsccts.svg&dl=1"  style="align: center" width="100" /> | Atividade de aula |
| :---------------------------------------------------------------------------------------------------------------------------- | :---------------- |

# Projeto: Utilizar um sistema de arquivos no ESP32

O Esp32 possui memória RAM interna e um chip externo de Memória FLASH. É nessa memória FLASH que os programas são armazenados. Existem algumas áreas dessa memória. Veja o diagrama da memória.

Podemos utilizar uma parte dessa memória FLASH para utilizar como aŕea de um sistema de arquivos. Para isso, cria-se um projeto e ajusta-se as configuração para usar a memória FLASH.

```text
idf.py create-project projeto-esp
cd projeto-esp
idf.py menuconfig
```

## Ajustar o tamanho da memória flash

A memória flash dos kits do ESP é de 4MB, assim, usando o menuconfig ajusta-se o tamanho para 4MB.

Acesse **Serial flasher config --->** e depois **Flash size (4MB) --->**

Retorne ao menu anterior e selecione **Partition table --->** e depois **Partition Table (Custom partition table CSV) --->** e selecione **(X) Custom partition table CSV**. Salve as alterações e saia do menuconfig.

Essas configurações informam que será utilizada uma tabela de partições determinada por um arquivo no diretório do projeto. Vamos criar esse arquivo **partitions.csv**

O conteúdo desse arquivo será a relação de partições. Segundo a figura, teremos uma partição nvs, phy, factory e agora teremos uma partição de sistema de arquivos que iremos chamar **storage**. A partição **storage** será alocada logo após a partição de programas (que ocupa 1MB) e vamos fazer o mesmo para a partição **storage**, assim nossa partição de sistema de arquivos irá ocupar 1MB.

```text
# ESP-IDF Partition Table
# Name,      Type,    SubType, Offset,    Size, Flags
nvs,         data,    nvs,     0x9000,    0x6000,
phy_init,    data,    phy,     0xf000,    0x1000,
factory,     app,     factory, 0x10000,   1M,
storage,     data,    spiffs,  0x110000,  0x100000,
```

Depois disso, deve-se criar um diretório (por exemplo: **partition**) dentro do diretório de projeto e dentro do **partition** vamos colocar arquivos que queremos acessar pelo ESP32. Crie um arquivo texto chamado **arquivo.txt**, escreva algum conteúdo nele e salve.

Agora é necessário editar o CMakeLists.txt que existe dentro do diretório **main** do projeto:

```text
idf_component_register(SRCS "projeto-esp.c"
                    INCLUDE_DIRS ".")
spiffs_create_partition_image(storage ../partition FLASH_IN_PROJECT)
```

## Escrevendo o código para usar o Sistema de Arquivos

A biblioteca do **spiffs** é um componente de software que está disponível na instalação padrão do idf (você pode encontrá-lo em ~/esp/components/spiffs). Deve-se incluir a biblioteca _esp_spiffs.h_

```C
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

    FILE * f = fopen ("/storage/arquivo.txt", "r");
    if (f == NULL)
    {
        printf("Nao consigo ler o arquivo\n");
        return;
    }

    char linha[64];
    fgets(linha,sizeof(linha),f);

    printf("Lido = %s\n",linha);
    fclose(f);
}
```

# Fazendo o FLASH de apenas uma partição

Podemos fazer o flash apenas da partição de programa ou apenas da partição de dados. O idf.py flash faz o flash de ambas.

Para fazer o flash apenas da partição de dados temos que fazer:

```
esptool.py  write_flash 0x110000 build/storage.bin
```

Essa linha informa qual o arquivo de partição será usado e qual o endereco inicial que ele será salvo na flash. Leia o **partition.csv** para ver o endereço inicial de cada partição.

Já para fazer o flash apenas da partição de programa:

```
esptool.py  write_flash 0x10000 build/projeto-esp.bin
```

O seguinte código conta a quantidade de boots que o ESP sofreu. Ele incrementa um contador e assim podemos fazer o flash apenas de uma partição de programa e ver que o contador não mudou. Faça esse teste e depois faça o **idf.py flash ** para que ele faça o flash de ambas as partições.

```
include <stdio.h>
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
```
