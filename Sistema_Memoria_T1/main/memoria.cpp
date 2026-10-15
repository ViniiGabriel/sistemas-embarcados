#include "memoria.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           GPIO_NUM_22
#define I2C_MASTER_SDA_IO           GPIO_NUM_23
#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          400000
#define I2C_MASTER_TIMEOUT_MS       1000

#define EEPROM_ADDR                 0x50
#define EEPROM_PAGE_SIZE            64

void memoria_init(void) {
    i2c_config_t conf;
    memset(&conf, 0, sizeof(i2c_config_t));
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void memoria_fwrite(void *buffer, uint16_t posicao, uint16_t tam) {
    uint8_t *data = (uint8_t *)buffer;
    uint16_t bytes_written = 0;

    while (bytes_written < tam) {
        uint16_t current_pos = posicao + bytes_written;
        uint16_t page_offset = current_pos % EEPROM_PAGE_SIZE;
        uint16_t bytes_to_page_end = EEPROM_PAGE_SIZE - page_offset;
        uint16_t chunk_size = tam - bytes_written;

        if (chunk_size > bytes_to_page_end) {
            chunk_size = bytes_to_page_end;
        }

        // buffer requires 2 bytes for the internal EEPROM address, plus the data
        uint8_t write_buf[EEPROM_PAGE_SIZE + 2];
        write_buf[0] = (current_pos >> 8) & 0xFF;
        write_buf[1] = current_pos & 0xFF;

        for (uint16_t i = 0; i < chunk_size; i++) {
            write_buf[2 + i] = data[bytes_written + i];
        }

        i2c_master_write_to_device(I2C_MASTER_NUM, EEPROM_ADDR, write_buf, chunk_size + 2, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));

        // EEPROM needs time to write the page to non-volatile memory
        vTaskDelay(pdMS_TO_TICKS(10));

        bytes_written += chunk_size;
    }
}

void memoria_fread(void *buffer, uint16_t posicao, uint16_t tam) {
    uint8_t *data = (uint8_t *)buffer;
    uint8_t addr_buf[2];

    addr_buf[0] = (posicao >> 8) & 0xFF;
    addr_buf[1] = posicao & 0xFF;

    // the 24C256 auto-increments the address pointer on reads, so we don't need to chunk page boundaries here
    i2c_master_write_read_device(I2C_MASTER_NUM, EEPROM_ADDR, addr_buf, 2, data, tam, pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
}
