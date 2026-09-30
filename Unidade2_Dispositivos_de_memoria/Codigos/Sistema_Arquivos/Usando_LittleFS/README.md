
 # LittleFS
 
 O LittleFS é um outro projeto de sistema de arquivos, concorrente do spiffs. O LittleFS possui várias vantagens:
 
 * É um projeto melhor mantido;
 * O sistema de arquivos é mais tolerante a problemas;
 * O sistema de arquivos tem desempenho melhor;
 * O sistema de arquivos SUPORTA diretórios (ja o spiffs é chamado flat pois  todos os arquivos precisam ficar no mesmo diretório)
 
 
 Consulte mais informações em : https://github.com/wreyford/demo_esp_littlefs
 
 
 # Como utilizar
 
 Crie o seu projeto da forma usual.
 
 ~~~
 idf.py create-project projeto-esp
 cd projeto-esp
 ~~~

 No IDF 4.x o LittleFS vinha embutido.
 
 No IDF 5.x ele virou componente externo.
 
 Como estou usando o IDF 5.x, vou adicionar o componente ao projeto
 
~~~
idf.py add-dependency joltwallet/littlefs

~~~

Depois de adicionar ele ao projeto, podemos rodar o **idf.py menuconfig**, ir ate o item **Component config** e então **LittleFS**

Isso mostra que o componente foi instalado e nao precisa mudar nada se não desejar. 

A memória flash dos kits do ESP é de 4MB, assim, usando o menuconfig ajusta-se o tamanho para 4MB.

Acesse **Serial flasher config --->** e depois **Flash size (4MB) --->**

Retorne ao menu anterior e selecione **Partition table --->** e depois **Partition Table (Custom partition table CSV) --->** e selecione **(X) Custom partition table CSV**. Salve as alterações e saia do menuconfig.

Essas configurações informam que será utilizada uma tabela de partições determinada por um arquivo no diretório do projeto. Vamos criar esse arquivo **partitions.csv**

~~~
# Name,     Type, SubType, Offset,   Size
nvs,        data, nvs,     0x9000,   0x5000
phy_init,   data, phy,     0xe000,   0x1000
factory,    app,  factory, 0x10000,  1M
littlefs,   data, littlefs,,        1M

~~~

Crie uma pasta do diretório do projeto para representar a sua partição. Neste exemplo será chamado de 
MEU_HD/


edite o arquivo main/CMakeLists.txt  e adicione littlefs_create_partition_image(littlefs ../MEU_HD FLASH_IN_PROJECT)

~~~
idf_component_register(SRCS "projeto-esp.c"
                    INCLUDE_DIRS ".")

littlefs_create_partition_image(littlefs ../MEU_HD FLASH_IN_PROJECT)

~~~
