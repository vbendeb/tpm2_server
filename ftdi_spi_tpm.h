// Copyright 2015 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TRUNKS_TRUNKS_FTDI_SPI_H_
#define TRUNKS_TRUNKS_FTDI_SPI_H_

#include "config.h"
#include "mpsse.h"

typedef enum {
  false = 0,
  true,
} bool;

int FtdiSpiInit(uint32_t freq, int enable_debug);
void FtdiStop(void);
size_t FtdiSendCommandAndWait(uint8_t *tpm_command, size_t command_size);

#endif  // TRUNKS_TRUNKS_FTDI_SPI_H_
