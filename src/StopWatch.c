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

#include "StopWatch.h"
double real_time()
{

#if defined(_WIN32)

    LARGE_INTEGER startTime;
    LARGE_INTEGER frequency;    QueryPerformanceFrequency(&frequency);    QueryPerformanceCounter(&startTime);    return (double)startTime.QuadPart / (double)frequency.QuadPart;#elif defined(__unix__) || defined(__APPLE__) && defined(__MACH__)
/* AIX, BSD, Cygwin, HP-UX, Linux, OSX, POSIX, Solaris. ----- */
    struct timeval tm;
    gettimeofday(&tm, NULL);    return (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;#else
/* Unsupported platform ------------------------------------- */
    return -1.0;

#endif}
