/***************************************************
* Alt replacements for several C string functions. *
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_STRINGUTILS_H_
#define _MHC_STRINGUTILS_H_
#include "MHC_config.h"

/** Case-insensitive string comparison */
DLL_PUBLIC int cistrncmp(const char * str1, const char * str2, length_t num);

/** Get length of next token. Default delimiter list is " \r\n" (space and newlines). Used as a replacement for strtok & strlen. */
DLL_PUBLIC length_t strtoklen(const char * str, const char * delimiters);

/** Seek to next token in a given string. Returns a pointer to the start of it, or NULL if one cannot be found. Optionally sets the token length as well. */
DLL_PUBLIC const char * strseektok(const char * str, length_t * tokLenOut, const char * delimiters);

/** Copy string to buffer (like strncpy, but without needing the C standard library).
 * Skips any terminating null character, even if present within src and srcCount.
 */
DLL_PUBLIC length_t bufncpy(byte_t * dest, const length_t destLen, const byte_t * src, const length_t srcCount);

/** Fill <count> bytes of dest with the given value, i.e. a memset replacement. */
DLL_PUBLIC byte_t * bufnset(byte_t * dest, const length_t count, byte_t value);

/** Copy an array of string tokens to dest, with an optional separator in between. Null-terminates dest whether or not a terminator is given. */
DLL_PUBLIC length_t tokncpy(byte_t * dest, const length_t destLen, const char ** tokens, const length_t tokenCount, const char * separator, const char * terminator);

/** Converts a base-10 ASCII number to u16. Returns 0 if no valid digits are found. */
DLL_PUBLIC u16_t strtou16(const char * str, const length_t strLen);

/** Converts a u16 to a base-10 ASCII number. Returns 0 and doesn't write if there's not enough space in dest. */
DLL_PUBLIC length_t u16tostr(u16_t val, char * dest, const length_t destLen);

#endif //_MHC_STRINGUTILS_H_
