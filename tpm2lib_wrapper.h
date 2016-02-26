#ifndef __FTDI_TPM2LIB_WRAPPER_C
#define __FTDI_TPM2LIB_WRAPPER_C

#include <stdint.h>
#include <sys/types.h>

#include "config.h"

int Tpm2LibInit(uint32_t freq, int debug);
size_t Tpm2LibProcess(uint8_t *message, size_t message_size);

#endif // __FTDI_TPM2LIB_WRAPPER_C
