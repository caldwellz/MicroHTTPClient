/***************************************************
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/
#include "MHC_requests.h"
#include "MHC_helpers.h"
#include "MHC_stringutils.h"

static MHC_params sharedParams;
static MHC_response res;
static byte_t reqBuf[MHC_REQUEST_BUFFER_SIZE];
static byte_t resBuf[MHC_RESPONSE_BUFFER_SIZE];

MHC_response* MHC_directRequest(MHC_context* ctx, MHC_params* params) {
  char u16Str[7];
  if (ctx == NULL || params == NULL)
    return NULL;

  // Request string
  const char * headTokens[3] = {
    params->method,
    params->path,
    "HTTP/1.1"
  };
  length_t reqLen = tokncpy(reqBuf, MHC_REQUEST_BUFFER_SIZE, headTokens, 3, " ", "\r\n");

  // Host, including port if non-default
  reqLen += MHC_addHeader(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, HEADER_HOST, params->hostname);
  if (params->port != 80) {
    u16Str[0] = ':';
    u8_t u16StrLen = u16tostr(params->port, u16Str + 1, 6) + 1;
    reqLen -= 2; // Erase the header CRLF
    reqLen += bufncpy(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, u16Str, u16StrLen);
    reqLen += bufncpy(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, "\r\n", 2);
  }

  // User-Agent
  reqLen += MHC_addHeader(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, HEADER_USER_AGENT, MHC_USER_AGENT);

  // Media type headers
  media_type_t media = params->accept;
  if (media > MEDIA_TYPE_NONE && media < _MEDIA_TYPE_RESERVED)
    reqLen += MHC_addMediaType(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, HEADER_ACCEPT, media);
  media = params->contentType;
  if (media > MEDIA_TYPE_NONE && media < _MEDIA_TYPE_RESERVED)
    reqLen += MHC_addMediaType(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, HEADER_CONTENT_TYPE, media);

  // Request body
  if (params->bodyLen > 0) {
    u16tostr(params->bodyLen, u16Str, 7);
    reqLen += MHC_addHeader(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, HEADER_CONTENT_LENGTH, u16Str);
    reqLen += bufncpy(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, "\r\n", 2);
    reqLen += bufncpy(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, params->body, params->bodyLen);
  }

  // Clear the remainder of the buffers and make sure they still get null-terminated if they are full.
  // TODO: Check if they are too full and return a status as appropriate
  bufnset(reqBuf + reqLen, MHC_REQUEST_BUFFER_SIZE - reqLen, '\0');
  bufnset(resBuf, MHC_RESPONSE_BUFFER_SIZE, '\0');
  reqBuf[MHC_REQUEST_BUFFER_SIZE - 1] = '\0';

  // Fill response
  // FIXME: The response is currently just set to the request values for testing purposes
  res.status = 200;
  res.contentType = media;
  res.body = reqBuf;
  res.bodyLen = reqLen;

  return &res;
}

MHC_response* MHC_request(MHC_context* ctx, const char* url, MHC_params* params) {
  // Sanity checks
  if (ctx == NULL || url == NULL || params == NULL)
    return NULL;
  if (params->hostname && params->path)
    return MHC_directRequest(ctx, params);

  // Use the response buffer as temp storage for the host and path
  length_t bufLen = MHC_RESPONSE_BUFFER_SIZE / 2;
  params->hostname = resBuf;
  params->path = resBuf + bufLen;
  host_error_t error = MHC_getHostFromURL(url, sharedParams.hostname, bufLen, &sharedParams.port, sharedParams.path, bufLen);
  if (error != HOST_OK)
    return NULL;

  MHC_response * res = MHC_directRequest(ctx, params);
  params->hostname = NULL;
  params->path = NULL;
  return res;
}

MHC_response* MHC_get(MHC_context* ctx, const char* url, media_type_t accept) {
  sharedParams.method = "GET";
  sharedParams.methodLen = 3;
  sharedParams.accept = accept;
  sharedParams.contentType = MEDIA_TYPE_NONE;
  sharedParams.body = NULL;
  sharedParams.bodyLen = 0;
  return MHC_request(ctx, url, &sharedParams);
}

MHC_response* MHC_put(MHC_context* ctx, const char* url, const byte_t* body, length_t bodyLen, media_type_t accept, media_type_t contentType) {
  sharedParams.method = "PUT";
  sharedParams.methodLen = 3;
  sharedParams.accept = accept;
  sharedParams.contentType = contentType;
  sharedParams.body = body;
  sharedParams.bodyLen = bodyLen;
  return MHC_request(ctx, url, &sharedParams);
}

MHC_response* MHC_post(MHC_context* ctx, const char* url, const byte_t* body, length_t bodyLen, media_type_t accept, media_type_t contentType) {
  sharedParams.method = "POST";
  sharedParams.methodLen = 4;
  sharedParams.accept = accept;
  sharedParams.contentType = contentType;
  sharedParams.body = body;
  sharedParams.bodyLen = bodyLen;
  return MHC_request(ctx, url, &sharedParams);

}

MHC_response* MHC_patch(MHC_context* ctx, const char* url, const byte_t* body, length_t bodyLen, media_type_t accept, media_type_t contentType) {
  sharedParams.method = "PATCH";
  sharedParams.methodLen = 5;
  sharedParams.accept = accept;
  sharedParams.contentType = contentType;
  sharedParams.body = body;
  sharedParams.bodyLen = bodyLen;
  return MHC_request(ctx, url, &sharedParams);
}

MHC_response* MHC_delete(MHC_context* ctx, const char* url) {
  sharedParams.method = "DELETE";
  sharedParams.methodLen = 6;
  sharedParams.accept = MEDIA_TYPE_NONE;
  sharedParams.contentType = MEDIA_TYPE_NONE;
  sharedParams.body = NULL;
  sharedParams.bodyLen = 0;
  return MHC_request(ctx, url, &sharedParams);
}
