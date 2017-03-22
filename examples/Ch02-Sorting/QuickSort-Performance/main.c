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
#include <time.h>
#include <stdlib.h>
#include "IntegerSequence.h"
#include "StopWatch.h"

int compare(int a, int b)
{
	return a - b;
}

int main(int argc, char *argv[])
{
	IntegerSequence s;

	/* initialize vector, default capacity: 4 */
	sequence_init(&s);

	int n = 200;
	double dt1 = 0.0, dt2;

	while (n < 1000000) {
		sequence_clear(&s);
		/* populate vector with random numbers */
		srand(time(NULL));
		for (int i = 0; i < n; i++)
			sequence_push_back(&s, rand());
		/*print(&s); */

		/* quick sort */
		double t0 = real_time();
		sequence_quicksort(&s, compare);
		double t1 = real_time();
		dt2 = t1 - t0;
		double ratio = dt1 == 0.0 ? 0.0 : dt2 / dt1;
		printf("n: %7d, elapsed time: %lf sec, ratio: %lf\n", n, dt2,
		       ratio);
		dt1 = dt2;
		/*print(&s); */
		n *= 2;
	}

	sequence_destroy(&s);

	return 0;
}
