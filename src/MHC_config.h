/***************************************************
* config.h: Build-time configuration               *
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_CONFIG_H_
#define _MHC_CONFIG_H_

#ifdef BUILDING_LIBRARY
  #include <config.h>
  #include <version.h>
#else
  #define NDEBUG
  #define DLL_PUBLIC
  #define DLL_LOCAL
#endif

#endif //_MHC_CONFIG_H_
