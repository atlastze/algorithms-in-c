/* Copyright (C) 2017, kylinsage <kylinsage@gmail.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include "IntegerSequence.h"

int main(int argc, char *argv[])
{
    int a[] = { 1, 2, 3, 4, 5, 6 };
    IntegerSequence s;

    /* initialize sequence, default capacity: 4 */
    sequence_init(&s);
    sequence_write(stdout, &s, " ");

    /* populate sequence, reallocate capacity: 8 */
    sequence_push_back(&s, a[0]);
    sequence_push_back(&s, a[1]);
    sequence_push_back(&s, a[2]);
    sequence_push_back(&s, a[3]);
    sequence_push_back(&s, a[4]);
    sequence_push_back(&s, a[5]);
    sequence_write(stdout, &s, " ");

    /* insert elements, capacity is enough */
    sequence_insert(&s, 2, 100);
    sequence_insert(&s, 2, 200);
    sequence_write(stdout, &s, " ");

    /* insert elements, reallocate capacity: 16 */
    sequence_insert(&s, 2, 300);
    sequence_write(stdout, &s, " ");

    /* remove elements */
    sequence_remove(&s, 3, 4);
    sequence_write(stdout, &s, " ");

    sequence_destroy(&s);

    return 0;
}
