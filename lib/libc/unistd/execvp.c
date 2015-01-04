/**
 *******************************************************************************
 * @file    execvp.c
 * @author  Olli Vanhoja
 * @brief   Execute a file.
 * @section LICENSE
 * Copyright (c) 2014 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
 * Originally by the Regents of the University of California?
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <syscall.h>
#include <unistd.h>

static char shell[] = "/bin/sh";
#define _PATH_STDPATH "/usr/bin:/bin:/usr/sbin:/sbin:"

static char * execat(char * s1, const char * s2, char * si)
{
    char * s;

    s = si;
    while (*s1 && *s1 != ':') {
        *s++ = *s1++;
    }
    if (si != s)
        *s++ = '/';
    while (*s2) {
        *s++ = *s2++;
    }
    *s = '\0';

    return *s1 ? ++s1 : 0;
}

int execvp(const char * name, char * const argv[])
{
    char * pathstr;
    char * cp;
    char fname[128];
    char * newargs[256];
    int i;
    unsigned etxtbsy = 1;
    int eacces = 0;

    pathstr = getenv("PATH");
    if (!pathstr)
        pathstr = _PATH_STDPATH;
    cp = strchr(name, '/') ? "" : pathstr;

    do {
        cp = execat(cp, name, fname);
    retry:
        execv(fname, argv);
        switch (errno) {
        case ENOEXEC:
            newargs[0] = "sh";
            newargs[1] = fname;
            for (i = 1; (newargs[i + 1] = argv[i]); i++) {
                if (i >= 254) {
                    errno = E2BIG;
                    return -1;
                }
            }
            execv(shell, newargs);
            return -1;
        case ETXTBSY:
            if (++etxtbsy > 5)
                return -1;
            sleep(etxtbsy);
            goto retry;
        case EACCES:
            eacces++;
            break;
        case ENOMEM:
        case E2BIG:
            return -1;
        }
    } while (cp);

    if (eacces)
        errno = EACCES;
    return -1;
}