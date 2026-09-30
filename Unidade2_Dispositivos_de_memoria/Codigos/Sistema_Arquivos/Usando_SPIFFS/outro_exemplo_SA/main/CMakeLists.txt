idf_component_register(SRCS "aula.c"
                    INCLUDE_DIRS ".")
spiffs_create_partition_image(storage ../MINHA_PARTICAO FLASH_IN_PROJECT)