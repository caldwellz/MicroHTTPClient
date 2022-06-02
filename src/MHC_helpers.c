/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include "MHC_helpers.h"
#include "MHC_stringutils.h"

const char * media_types[] = {
  "",
  "*/*",
  "application/cbor",
  "application/json",
  "application/xml",
  "text/html",
  "text/plain",
  "application/octet-stream",
  "application/x-www-form-urlencoded",
  "multipart/form-data"
};

const char * header_list[] = {
  "",
  "Accept",
  "Content-Encoding",
  "Content-Type",
  "Content-Length",
  "Host",
  "Referrer",
  "User-Agent"
};

bool_t MHC_isErrorStatus(status_t status) {
  if (status >= STATUS_HTTP_CLIENT_ERROR)
    return TRUE;
  else
    return FALSE;
}

host_error_t MHC_getHostFromURL(const char * url, byte_t * hostname, length_t hostnameLen, u16_t * port, byte_t * path, length_t pathLen) {
  if (url == NULL || hostname == NULL || hostnameLen <= 0 || port == NULL || path == NULL || pathLen <= 0)
    return HOST_INVALID_PARAMS;

  // Only plain HTTP is currently supported
  const length_t protocolLen = 7;
  const char * urlPtr = url + protocolLen;
  if (cistrncmp(url, "http://", protocolLen) != 0)
    return HOST_INVALID_PROTOCOL;

  // Extract the hostname
  length_t tokLen = strtoklen(urlPtr, ":/");
  if (tokLen <= 0)
    return HOST_INVALID_HOSTNAME;
  bufncpy(hostname, hostnameLen, urlPtr, tokLen);
  hostname[MIN(tokLen, hostnameLen - 1)] = '\0';
  urlPtr += tokLen;

  // Extract the port override, if any
  char pathSeparator = urlPtr[0];
  if (pathSeparator == ':') {
    tokLen = strtoklen(++urlPtr, "/");
    if (tokLen <= 0 || tokLen > 5)
      return HOST_INVALID_PORT;
    *port = strtou16(urlPtr, tokLen);
    urlPtr += tokLen;
    pathSeparator = urlPtr[0];
  } else {
    *port = 80;
  }

  if (pathSeparator == '/') {
    tokLen = bufncpy(path, pathLen, urlPtr, pathLen);
    path[MIN(tokLen, pathLen - 1)] = '\0';
  } else {
    return HOST_INVALID_PATH;
  }

  return HOST_OK;
}

const char * MHC_getMediaString(media_type_t type) {
  if (type < MEDIA_TYPE_NONE || type >= _MEDIA_TYPE_RESERVED)
    return NULL;

  return media_types[type];
}

media_type_t MHC_identifyMediaType(const char * type, length_t typeLen) {
  if (type == NULL || typeLen == 0)
    return MEDIA_TYPE_NONE;

  for (u16_t i = 1; i < _MEDIA_TYPE_RESERVED; ++i) {
    if (cistrncmp(media_types[i], type, typeLen) == 0)
      return i;
  }

  return MEDIA_TYPE_OTHER;
}

length_t MHC_addHeader(byte_t * dest, const length_t destLen, header_t header, const char * content) {
  if (dest == NULL || destLen <= 0 || header <= HEADER_NONE || header >= _HEADER_RESERVED || content == NULL)
    return 0;

  const char * tokens[2] = {
    header_list[header],
    content
  };

  return tokncpy(dest, destLen, tokens, 2, ": ", "\r\n");
}

length_t MHC_addMediaType(byte_t * dest, const length_t destLen, header_t header, media_type_t type) {
  if (header != HEADER_ACCEPT && header != HEADER_CONTENT_TYPE)
    return 0;

  return MHC_addHeader(dest, destLen, header, MHC_getMediaString(type));
}
