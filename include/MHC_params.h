/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_PARAMS_H_
#define _MHC_PARAMS_H_
#include "MHC_config.h"
#include "MHC_helpers.h"

typedef struct MHC_params {
  byte_t * hostname;
  u16_t port;
  byte_t * path;
  const char * method;
  length_t methodLen;
  media_type_t accept;
  media_type_t contentType;
  byte_t * body;
  length_t bodyLen;
} MHC_params;

#endif //_MHC_PARAMS_H_
