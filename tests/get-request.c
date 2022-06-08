/***************************************************
* Test - GET request                               *
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

// Defines an example MHC_context (ctx)
#include "SDL_net_context.inc"

static const char url[] = "http://example.com/";

int main(void)
{
  MHC_response* res = MHC_get(&ctx, url, MEDIA_TYPE_HTML);
  assert(res);
  printf("[GET %s]\n[Status:%i ContentType:%i BodyLen:%i]\n--------------------------------------\n%s\n--------------------------------------\n",
    url, res->status, res->contentType, res->bodyLen, res->body);

  return 0;
}
