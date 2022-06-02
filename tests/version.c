/***************************************************
* Test - Project version / revision                *
* Copyright (C)                                    *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include <version.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Verify pointers not null
    assert(PROJECT_NAME);
    assert(PROJECT_VERSION);
    assert(PROJECT_REVISION);
    assert(PROJECT_COMPILER);
    assert(PROJECT_COMPILER_VERSION);

    // Verify contents not null
    assert(strlen(PROJECT_NAME));
    assert(strlen(PROJECT_VERSION));
    assert(strlen(PROJECT_REVISION));
    assert(strlen(PROJECT_COMPILER));
    assert(strlen(PROJECT_COMPILER_VERSION));

    // Log the version strings
    printf("%s %s-%s (%s-%s)\n", PROJECT_NAME, PROJECT_VERSION, PROJECT_REVISION, PROJECT_COMPILER, PROJECT_COMPILER_VERSION);

    return 0;
}
