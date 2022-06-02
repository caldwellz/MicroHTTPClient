/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MHC_HELPERS_H_
#define _MHC_HELPERS_H_
#include "MHC_config.h"

/** HTTP and internal status responses */
typedef enum status_t {
  STATUS_HTTP_MAX = 600,
  STATUS_HTTP_SERVER_ERROR = 500,
  STATUS_HTTP_CLIENT_ERROR = 400,
  STATUS_HTTP_REDIRECT = 300,
  STATUS_HTTP_OK = 200,
  STATUS_HTTP_INFORMATION = 100,
  STATUS_INVALID_PARAMS = -1,
  STATUS_CONNECTION_FAILED = -2,
  STATUS_INCOMPLETE_TRANSMIT = -3,
  STATUS_INCOMPLETE_RESPONSE = -4,
  STATUS_INVALID_RESPONSE = -5
} status_t;

/** Supported media types for identification */
typedef enum media_type_t {
  MEDIA_TYPE_NONE = 0,
  MEDIA_TYPE_ANY,
  MEDIA_TYPE_CBOR,
  MEDIA_TYPE_JSON,
  MEDIA_TYPE_XML,
  MEDIA_TYPE_HTML,
  MEDIA_TYPE_TEXT,
  MEDIA_TYPE_BINARY,
  MEDIA_TYPE_URLENCODED_FORM,
  MEDIA_TYPE_MULTIPART_FORM,
  _MEDIA_TYPE_RESERVED,
  MEDIA_TYPE_OTHER = 65534
} media_type_t;

typedef enum header_t {
  HEADER_NONE = 0,
  HEADER_ACCEPT,
  HEADER_CONTENT_ENCODING,
  HEADER_CONTENT_TYPE,
  HEADER_CONTENT_LENGTH,
  HEADER_HOST,
  HEADER_REFERRER,
  HEADER_USER_AGENT,
  _HEADER_RESERVED
} header_t;

typedef enum host_error_t {
  HOST_OK = 0,
  HOST_INVALID_PARAMS,
  HOST_INVALID_PROTOCOL,
  HOST_INVALID_HOSTNAME,
  HOST_INVALID_PORT,
  HOST_INVALID_PATH
} host_error_t;

DLL_PUBLIC bool_t MHC_isErrorStatus(status_t status);

DLL_PUBLIC host_error_t MHC_getHostFromURL(const char * url, byte_t * hostname, length_t hostnameLen, u16_t * port, byte_t * path, length_t pathLen);

DLL_PUBLIC const char * MHC_getMediaType(media_type_t type);

DLL_PUBLIC media_type_t MHC_identifyMediaType(const char * type, length_t typeLen);

DLL_PUBLIC length_t MHC_addHeader(byte_t * dest, const length_t destLen, header_t header, const char * content);

DLL_PUBLIC length_t MHC_addMediaType(byte_t * dest, const length_t destLen, header_t header, media_type_t type);

#endif //_MHC_HELPERS_H_
