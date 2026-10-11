#ifndef SISTEMA_ARQUIVOS_LINUX_H
#define SISTEMA_ARQUIVOS_LINUX_H

#include <stdio.h>
#include <stdint.h>
#include "sa_api.h"

void memoria_init();
void memoria_fwrite(void *buffer, uint16_t posicao, uint16_t tam);
void memoria_fread(void *buffer, uint16_t posicao, uint16_t tam);

#endif
