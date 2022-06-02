/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_RESPONSE_H_
#define _MHC_RESPONSE_H_
#include "MHC_config.h"
#include "MHC_helpers.h"

typedef struct MHC_response {
  status_t status;
  media_type_t contentType;
  byte_t* body;
  length_t bodyLen;
} MHC_response;

#endif //_MHC_RESPONSE_H_
