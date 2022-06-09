/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_CONFIG_H_
#define _MHC_CONFIG_H_

// ** Application-specific config **
// The buffer should be big enough to hold the largest of: a full request,
// a full response, or (2 * longest URL) + 25 if using auto URL decoding.
// Between 1k and 4k is usually sufficient, depending on expected content lengths.
#define MHC_BUFFER_SIZE 2048
#define MHC_USER_AGENT "MicroHTTPClient/0.0.1"

// Standard constants, types, and math things
#define FALSE 0
#define TRUE 1
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#ifndef NULL
  #define NULL ((void *) 0)
#endif
typedef unsigned char u8_t;
typedef signed char s8_t;
typedef unsigned short u16_t;
typedef signed short s16_t;
typedef u8_t bool_t;
typedef u8_t byte_t;
typedef s16_t length_t;

// Automatically handle library vs inline builds
#ifdef BUILDING_LIBRARY
  #include <config.h>
  #include <version.h>
#else
  #define DLL_PUBLIC
  #define DLL_LOCAL
  #ifndef DEBUG
    #define NDEBUG
  #endif
#endif

#endif //_MHC_CONFIG_H_
