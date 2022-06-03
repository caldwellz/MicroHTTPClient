/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_REQUESTS_H_
#define _MHC_REQUESTS_H_
#include "MHC_config.h"
#include "MHC_context.h"
#include "MHC_params.h"
#include "MHC_response.h"

DLL_PUBLIC status_t MHC_formatRequest(const byte_t* buf, length_t bufLen, MHC_params* params);
DLL_PUBLIC MHC_response* MHC_directRequest(MHC_context* ctx, MHC_params* params);
DLL_PUBLIC MHC_response* MHC_request(MHC_context* ctx, const char* url, MHC_params* params);
DLL_PUBLIC MHC_response* MHC_get(MHC_context* ctx, const char* url, media_type_t accept);
DLL_PUBLIC MHC_response* MHC_put(MHC_context* ctx, const char* url, const byte_t* body, length_t bodyLen, media_type_t accept, media_type_t contentType);
DLL_PUBLIC MHC_response* MHC_post(MHC_context* ctx, const char* url, const byte_t* body, length_t bodyLen, media_type_t accept, media_type_t contentType);
DLL_PUBLIC MHC_response* MHC_patch(MHC_context* ctx, const char* url, const byte_t* body, length_t bodyLen, media_type_t accept, media_type_t contentType);
DLL_PUBLIC MHC_response* MHC_delete(MHC_context* ctx, const char* url);

#endif //_MHC_REQUESTS_H_
