/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_CONTEXT_H_
#define _MHC_CONTEXT_H_
#include "MHC_config.h"

typedef void socket;

typedef struct MHC_context {
  socket* sock;
  socket* (*connect)(const char* host, u16_t port);
  void (*disconnect)(socket* sock);
  u16_t (*send)(socket* sock, const byte_t* data, length_t len);
  u16_t (*recv)(socket* sock, const byte_t* data, length_t maxLen);
} MHC_context;

#endif //_MHC_CONTEXT_H_
