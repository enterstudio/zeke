/**
 *******************************************************************************
 * @file    fcntl.h
 * @author  Olli Vanhoja
 * @brief   File control options.
 * @section LICENSE
 * Copyright (c) 2014 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
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

/* TODO http://pubs.opengroup.org/onlinepubs/9699919799/ */

/** @addtogroup LIBC
 * @{
 */

#pragma once
#ifndef FCNTL_H
#define FCNTL_H

#include <sys/cdefs.h>

/* cmd args used by fcntl() */
#define F_DUPFD         0   /*!< Duplicate file descriptor. */
#define F_DUPFD_CLOEXEC 1   /*!< Duplicate file descriptor with the
                              close-on- exec flag FD_CLOEXEC set. */
#define F_GETFD         2   /*!< Get file descriptor flags. */
#define F_SETFD         3   /*!< Set file descriptor flags. */
#define F_GETFL         4   /*!< Get file status flags and file access modes. */
#define F_SETFL         5   /*!< Set file status flags. */
#define F_GETLK         6   /*!< Get record locking information. */
#define F_SETLK         7   /*!< Set record locking information. */
#define F_SETLKW        8   /*!< Set record locking information; wait if
                             *   blocked. */
#define F_GETOWN        9   /*!< Get process or process group ID to receive
                              SIGURG signals. */
#define F_SETOWN        10  /*!< Set process or process group ID to receive
                             *   SIGURG signals. */

/* fcntl() fd flag */
#define FD_CLOEXEC      1   /*!< Close the file descriptor upon execution of
                             *   an exec family function. */

/* fcntl() l_type arg */
#define F_RDLCK         0   /*!< Shared or read lock. */
#define F_UNLCK         1   /*!< Unlock. */
#define F_WRLCK         2   /*!< Exclusive or write lock. */

/* open() oflags */
#define O_CLOEXEC       0x0001 /*!< Close the file descriptor upon execution of
                                *   an exec family function. */
#define O_CREAT         0x0002 /*!< Create file if it does not exist. */
#define O_DIRECTORY     0x0004 /*!< Fail if not a directory. */
#define O_EXCL          0x0008 /*!< Exclusive use flag. */
#define O_NOCTTY        0x0010 /*!< Do not assign controlling terminal. */
#define O_NOFOLLOW      0x0020 /*!< Do not follow symbolic links. */
#define O_TRUNC         0x0040 /*!< Truncate flag. */
#define O_TTY_INIT      0x0080

/* open() file status flags */
#define O_APPEND        0x0100 /*!< Set append mode. */
#define O_NONBLOCK      0x0200 /*!< Non-blocking mode. */
#define O_SYNC          0x0400

#define O_ACCMODE       0x7000 /*!< Mask for file access modes. */
/* File access modes */
#define O_RDONLY        0x1000 /*!< Open for reading only. */
#define O_WRONLY        0x2000 /*!< Open for writing only. */
#define O_RDWR          0x3000 /*!< Open for reading and writing. */
#define O_SEARCH        0x0000 /*!< Open directory for search only. */
#define O_EXEC          0x4000 /*!< Open for execute only. */

#ifndef KERNEL_INTERNAL
__BEGIN_DECLS
int open(const char *, int, ...);
int creat(const char *, mode_t);
int fcntl(int, int, ...);
__END_DECLS
#endif /* !KERNEL_INTERNAL */

#endif /* FCNTL_H */

/**
 * @}
 */
