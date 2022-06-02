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
  const char * body = "{\"foo\":true,\"bar\":2}";
  MHC_context context;
  MHC_response* res = MHC_post(&context, "http://foo.bar:40/?page=1", body, strlen(body), MEDIA_TYPE_CBOR, MEDIA_TYPE_JSON);
  assert(res);
  printf("[Status:%i ContentType:%i BodyLen:%i]\n--------------------------------------\n%s\n--------------------------------------\n",
    res->status, res->contentType, res->bodyLen, res->body);

  return 0;
}
