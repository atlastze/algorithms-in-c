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
#include "Polynomial.h"

void display(Polynomial * p)
{
    slist_for_each(position, p) {
        if (COEF(position) > 0)
            printf("+");
        printf("%d*a^%d ", COEF(position), EXP(position));
    }
}

int main(int argc, char *argv[])
{
    int a[] = { 1, 2, 3, 4, 5 };
    Polynomial p1;
    Polynomial p2;
    Polynomial p;

    slist_init(&p1);
    slist_init(&p2);
    slist_init(&p);

    add_term(&p1, 1, 0);
    add_term(&p1, 5, 3);
    add_term(&p1, 2, 8);
    add_term(&p1, 3, 14);

    add_term(&p2, -5, 3);
    add_term(&p2, 10, 6);
    add_term(&p2, -3, 10);
    add_term(&p2, 8, 14);

    plus(&p1, &p2, &p);

    printf("P1(X) = ");
    display(&p1);
    printf("\n");
    printf("P2(X) = ");
    display(&p2);
    printf("\n");
    printf("P1(X)+P2(X) = ");
    display(&p);
    printf("\n");

    slist_destroy(&p1, free);
    slist_destroy(&p2, free);
    slist_destroy(&p, free);

    return 0;
}
