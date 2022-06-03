/***************************************************
* Test - POST request                              *
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include "MHC_requests.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  byte_t reqBuf[MHC_REQUEST_BUFFER_SIZE];
  MHC_params params = {
    .hostname = "example.com",
    .port = 80,
    .path = "/baz",
    .method = "POST",
    .methodLen = 4,
    .accept = MEDIA_TYPE_NONE,
    .contentType = MEDIA_TYPE_JSON,
    .body = "{\"foo\":true,\"bar\":2}",
    .bodyLen = 24,
  };

  assert(STATUS_INVALID_PARAMS == MHC_formatRequest(NULL, MHC_REQUEST_BUFFER_SIZE, &params));
  assert(STATUS_INVALID_PARAMS == MHC_formatRequest(reqBuf, MHC_REQUEST_BUFFER_SIZE, NULL));
  assert(STATUS_INVALID_PARAMS == MHC_formatRequest(reqBuf, 0, &params));
  assert(STATUS_BUFFER_EXCEEDED == MHC_formatRequest(reqBuf, 1, &params));
  assert(STATUS_SUCCESS == MHC_formatRequest(reqBuf, MHC_REQUEST_BUFFER_SIZE, &params));
  printf("%s\n", reqBuf);

  return 0;
}
