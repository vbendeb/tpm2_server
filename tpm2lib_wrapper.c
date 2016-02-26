
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ExecCommand_fp.h>
#include <PlatformData.h>
#include <Manufacture_fp.h>
#include <Platform.h>
#include <_TPM_Init_fp.h>

#include "tpm2lib_wrapper.h"

static int enable_debug;

static void dump_message(const char *prefix,
                         uint8_t *message, size_t message_size)
{
  uint32_t length, type, i;

  if (!enable_debug)
    return;

  if (message_size < 10) {
    printf("misformatted %s, %zd bytes\n", prefix, message_size);
    return;
  }

  memcpy(&length, message + 2, sizeof(length));
  memcpy(&type, message + 6, sizeof(type));

  length = be32toh(length);
  type = be32toh(type);

  printf("%s of type %3x, length %d", prefix, type, length);
  if (length != message_size)
    printf(" message size %zd", message_size);
  for (i = 0; i < message_size; i++) {
    if (!(i % BYTES_PER_LINE))
      printf("\n");
    printf(" %2.2x", message[i]);
  }
  printf("\n");
}

int Tpm2LibInit(uint32_t freq, int debug)
{
  /* TPM2 library initialization functions. */
  printf("Initiazling tpm2 simulator\n");
  _plat__Signal_PowerOn();
   _plat__NVEnable(NULL);
  if (TPM_Manufacture(TRUE)) {
    printf("Manufacture Initialization failed\n");
    return 1;
  }
  if (TPM_Manufacture(FALSE) != 1) {
    printf("Manufacture Re-Initialization failed\n");
    return 1;
  }
  TPM_TearDown();
  if(TPM_Manufacture(TRUE)) {
    printf("Teardown failed\n");
    return 1;
  }
  // Disable NV memory
  _plat__NVDisable();
  _TPM_Init();
  _plat__SetNvAvail();

  enable_debug = debug;
  return 1;
}



size_t Tpm2LibProcess(uint8_t *message, size_t message_size)
{
  void *response;
  unsigned response_size;

  dump_message("Command", message, message_size);
  ExecuteCommand(message_size, message, &response_size, (uint8_t **)&response);
  memset(message, 0xaa, 4096);
  memcpy(message, response, response_size);
  dump_message("Response", message, response_size);

  return response_size;
}
