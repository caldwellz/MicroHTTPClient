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

static const char MHC_HTTP_VERSION[] = "HTTP/1.1";
static MHC_params sharedParams;
static MHC_response sharedResponse;
static byte_t reqBuf[MHC_REQUEST_BUFFER_SIZE];
static byte_t resBuf[MHC_RESPONSE_BUFFER_SIZE];

status_t MHC_formatRequest(const byte_t* buf, length_t bufLen, MHC_params* params) {
  char u16Str[7];
  if (buf == NULL || bufLen <= 0 || params == NULL)
    return STATUS_INVALID_PARAMS;

  // Request string
  const char * headTokens[3] = {
    params->method,
    params->path,
    MHC_HTTP_VERSION
  };
  length_t bufIndex = tokncpy(buf, bufLen, headTokens, 3, " ", "\r\n");

  // Host, including port if non-default
  bufIndex += MHC_addHeader(buf + bufIndex, bufLen - bufIndex, HEADER_HOST, params->hostname);
  if (params->port != 80) {
    u16Str[0] = ':';
    u8_t u16StrLen = u16tostr(params->port, u16Str + 1, 6) + 1;
    bufIndex -= 2; // Erase the header CRLF
    bufIndex += bufncpy(buf + bufIndex, bufLen - bufIndex, u16Str, u16StrLen);
    bufIndex += bufncpy(buf + bufIndex, bufLen - bufIndex, "\r\n", 2);
  }

  // User-Agent and Connection
  bufIndex += MHC_addHeader(buf + bufIndex, bufLen - bufIndex, HEADER_USER_AGENT, MHC_USER_AGENT);
  bufIndex += MHC_addHeader(buf + bufIndex, bufLen - bufIndex, HEADER_CONNECTION, "close");

  // Media type headers
  media_type_t media = params->accept;
  if (media > MEDIA_TYPE_NONE && media < _MEDIA_TYPE_RESERVED)
    bufIndex += MHC_addMediaType(buf + bufIndex, bufLen - bufIndex, HEADER_ACCEPT, media);
  media = params->contentType;
  if (media > MEDIA_TYPE_NONE && media < _MEDIA_TYPE_RESERVED)
    bufIndex += MHC_addMediaType(buf + bufIndex, bufLen - bufIndex, HEADER_CONTENT_TYPE, media);

  // Request body
  if (params->bodyLen > 0) {
    u16tostr(params->bodyLen, u16Str, 7);
    bufIndex += MHC_addHeader(buf + bufIndex, bufLen - bufIndex, HEADER_CONTENT_LENGTH, u16Str);
    bufIndex += bufncpy(buf + bufIndex, bufLen - bufIndex, "\r\n", 2);
    bufIndex += bufncpy(buf + bufIndex, bufLen - bufIndex, params->body, params->bodyLen);
  } else {
    bufIndex += bufncpy(buf + bufIndex, bufLen - bufIndex, "\r\n", 2);
  }

  // The bufncpy and MHC_add* calls above will fill up to, but never past, the end of buf,
  // and will not include a null terminator. So, simply verify that there's still space for one.
  if (bufIndex >= bufLen)
    return STATUS_BUFFER_EXCEEDED;

  // Null out the remainder of the buffer
  bufnset(buf + bufIndex, bufLen - bufIndex, '\0');

  return STATUS_SUCCESS;
}

status_t MHC_parseResponse(MHC_response* res, const byte_t* buf, const length_t bufLen) {
  if (res == NULL || buf == NULL || bufLen <= 0)
    return STATUS_INVALID_PARAMS;

  // Check HTTP version header
  const length_t versionLen = sizeof(MHC_HTTP_VERSION) - 1;
  if (cistrncmp(buf, MHC_HTTP_VERSION, versionLen) != 0)
    return STATUS_INVALID_RESPONSE;

  // Status code and default content type
  length_t bufIndex = versionLen;
  length_t statusLen = strtoklen(buf + bufIndex, "\r\n");
  res->status = strtou16(buf + bufIndex, statusLen);
  res->contentType = MEDIA_TYPE_NONE;
  bufIndex += statusLen;

  // Seek to and loop through headers
  header_t type = _HEADER_RESERVED;
  do {
    // Skip CRLF and verify the response data hasn't ended prematurely
    if (buf[bufIndex] == '\r')
      ++bufIndex;
    ++bufIndex;
    if (bufIndex >= bufLen)
      return STATUS_INCOMPLETE_RESPONSE;

    byte_t* contentPtr;
    length_t contentLen;
    bufIndex += MHC_parseHeader(buf + bufIndex, bufLen - bufIndex, &type, &contentPtr, &contentLen);
    switch (type) {
      case HEADER_CONTENT_TYPE:
        res->contentType = MHC_identifyMediaType(contentPtr, contentLen);
        break;
      case HEADER_NONE:
        res->body = contentPtr;
        res->bodyLen = contentLen;
        break;
      default:
        // Unknown header
        break;
    }
  } while (type != HEADER_NONE);

  return res->status;
}

MHC_response* MHC_directRequest(MHC_context* ctx, MHC_params* params) {
  if (ctx == NULL || params == NULL)
    return NULL;

  ctx->sock = ctx->connect(params->hostname, params->port);
  if (ctx->sock == NULL)
    return NULL;

  status_t status = MHC_formatRequest(reqBuf, MHC_REQUEST_BUFFER_SIZE, params);
  if (status != STATUS_SUCCESS) {
    ctx->disconnect(ctx->sock);
    return NULL;
  }

  length_t reqLen = strtoklen(reqBuf, "\0");
  length_t sentLen = ctx->send(ctx->sock, reqBuf, reqLen);
  if (sentLen != reqLen) {
    ctx->disconnect(ctx->sock);
    return NULL;
  }

  // Fill response buffer, verifying no overflow by using the fact that a spot needs to remain for a null terminator
  length_t recvLen, resLen = 0;
  do {
    recvLen = ctx->recv(ctx->sock, resBuf + resLen, MHC_RESPONSE_BUFFER_SIZE - resLen);
    resLen += recvLen;
  } while(recvLen > 0);
  ctx->disconnect(ctx->sock);
  ctx->sock = NULL;
  if (resLen >= MHC_RESPONSE_BUFFER_SIZE)
    return NULL;

  resBuf[resLen] = '\0';
  MHC_parseResponse(&sharedResponse, resBuf, resLen);

  return &sharedResponse;
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
