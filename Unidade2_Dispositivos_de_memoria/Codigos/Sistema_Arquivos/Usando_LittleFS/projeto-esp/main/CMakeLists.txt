idf_component_register(SRCS "projeto-esp.c"
                    INCLUDE_DIRS ".")

littlefs_create_partition_image(littlefs ../littlefs FLASH_IN_PROJECT)
