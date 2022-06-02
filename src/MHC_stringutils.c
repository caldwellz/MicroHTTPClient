/***************************************************
* Alt replacements for several C string functions  *
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include "MHC_stringutils.h"

int cistrncmp(const char * str1, const char * str2, length_t num) {
  // Sanity checks
  if (str1 == str2 || num == 0)
    return 0;
  if (str1 == NULL)
    return -1;
  if (str2 == NULL)
    return 1;

  for (length_t i = 0; i < num; ++i) {
    s8_t diff = str1[i] - str2[i];
    if (str1[i] == '\0' || str2[i] == '\0')
      return diff;

    if (diff == 0)
      continue;

    // Case insensitivity
    if (diff == 32) {
      if (str1[i] >= 'a' && str1[i] <= 'z')
        continue;
    } else if (diff == -32) {
      if (str1[i] >= 'A' && str1[i] <= 'Z')
        continue;
    }

    return diff;
  }
  return 0;
}

length_t strtoklen(const char * str, const char * delimiters) {
  if (str == NULL)
    return 0;
  if (delimiters == NULL)
    delimiters = " \r\n";

  length_t len = 0;
  char nextChar = str[0];
  while (nextChar != '\0') {
    for (length_t n = 0; delimiters[n] != '\0'; ++n) {
      if (nextChar == delimiters[n])
        return len;
    }
    nextChar = str[++len];
  }
  return len;
}

const char * strseektok(const char * str, length_t * tokLenOut, const char * delimiters) {
  if (str == NULL)
    return NULL;

  char * tokPtr = str;
  length_t tokLen = strtoklen(tokPtr, delimiters);

  // Loop past delimiter characters
  while(tokLen == 0) {
    if (tokPtr[0] == '\0')
      return NULL;

    tokLen = strtoklen(++tokPtr, delimiters);
  }

  if (tokLenOut != NULL)
    *tokLenOut = tokLen;

  return tokPtr;
}

length_t bufncpy(byte_t * dest, const length_t destLen, const byte_t * src, const length_t srcCount) {
  if (dest == NULL || destLen <= 0 || src == NULL || srcCount <= 0)
    return 0;

  length_t maxCount = MIN(destLen, srcCount);
  length_t count = 0;
  for (; count < maxCount && src[count] != '\0'; ++count) {
    dest[count] = src[count];
  }
  return count;
}

byte_t * bufnset(byte_t * dest, const length_t count, byte_t value) {
  for (length_t n = 0; n < count; ++n) {
    dest[n] = value;
  }
  return dest;
}

length_t tokncpy(byte_t * dest, const length_t destLen, const char ** tokens, const length_t tokenCount, const char * separator, const char * terminator) {
  if (dest == NULL || destLen <= 0 || tokens == NULL || tokenCount <= 0)
    return 0;

  length_t destIndex = 0;
  for (u8_t tokIndex = 0; tokIndex < tokenCount && destIndex < destLen && tokens[tokIndex] != NULL; ++tokIndex) {
    if (tokIndex > 0) {
      destIndex += bufncpy(dest + destIndex, destLen - destIndex, separator, destLen);
    }
    destIndex += bufncpy(dest + destIndex, destLen - destIndex, tokens[tokIndex], strtoklen(tokens[tokIndex], "\0"));
  }
  destIndex += bufncpy(dest + destIndex, destLen - destIndex, terminator, destLen);

  const length_t destEnd = MIN(destIndex, destLen - 1);
  dest[destEnd] = '\0';
  return destEnd;
}

u16_t strtou16(const char * str, const length_t strLen) {
  if (str == NULL || strLen <= 0)
    return 0;

  u16_t val = 0;
  for (length_t n = 0; n < strLen; ++n) {
    const char c = str[n];
    if (c == '\0')
      break;

    if (c >= '0' && c <= '9') {
      val = (val * 10) + (c - 48);
    } else {
      // Skip past non-digits at the beginning, and stop when the digits end
      if (val != 0)
        break;
    }
  }
  return val;
}

length_t u16tostr(u16_t val, char * dest, const length_t destLen) {
  u8_t digits = 1;
  u16_t divisor = 1;
  for (u16_t tmpVal = val / 10; tmpVal > 0; digits++) {
    tmpVal /= 10;
    divisor *= 10;
  }

  if (dest == NULL || destLen < digits + 1)
    return 0;

  for (u8_t digitIndex = 0; digitIndex < digits; ++digitIndex) {
    const u8_t digitVal = val / divisor;
    dest[digitIndex] = digitVal + 48;
    val %= divisor;
    divisor /= 10;
  }
  dest[digits] = '\0';

  return digits;
}
