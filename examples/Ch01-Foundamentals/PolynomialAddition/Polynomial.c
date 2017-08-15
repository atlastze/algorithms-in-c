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

#include "Polynomial.h"

void add_term(Polynomial * position, int coef, int exp)
{
    PolynomialTerm *element = (PolynomialTerm *) malloc(sizeof(PolynomialTerm));
    element->coef = coef;
    element->exp = exp;
    slist_insert_back(position, element);
}

void plus(Polynomial * p1, Polynomial * p2, Polynomial * result)
{
    /* positions */
    Polynomial *term1 = p1;
    Polynomial *term2 = p2;
    Polynomial *term3 = result;

    while (term1->next && term2->next) {
        if (EXP(term1) == EXP(term2)) {
            int coef = COEF(term1) + COEF(term2);
            int exp = EXP(term1);
            /* advance */
            term1 = term1->next;
            term2 = term2->next;
            if (coef != 0) {
                add_term(term3, coef, exp);
                term3 = term3->next;
            }
        } else if (EXP(term1) > EXP(term2)) {
            int coef = COEF(term1);
            int exp = EXP(term1);
            add_term(term3, coef, exp);
            /* advance */
            term1 = term1->next;
            term3 = term3->next;
        } else {
            int coef = COEF(term2);
            int exp = EXP(term2);
            add_term(term3, coef, exp);
            /* advance */
            term2 = term2->next;
            term3 = term3->next;
        }

    }

    if (term1->next) {
        slist_for_each(position, term1) {
            add_term(term3, COEF(position), EXP(position));
            term3 = term3->next;
        }
    }

    if (term2->next) {
        slist_for_each(position, term2) {
            add_term(term3, COEF(position), EXP(position));
            term3 = term3->next;
        }
    }

}
