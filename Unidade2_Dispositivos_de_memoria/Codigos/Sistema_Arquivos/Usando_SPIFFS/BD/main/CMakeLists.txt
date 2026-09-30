idf_component_register(SRCS "projeto-esp.cpp" "BD.cpp"
                    INCLUDE_DIRS ".")
spiffs_create_partition_image(storage ../partition FLASH_IN_PROJECT)
