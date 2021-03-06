/**
 *******************************************************************************
 * @file    stat.h
 * @author  Olli Vanhoja
 * @brief   Data returned by the stat() function.
 * @section LICENSE
 * Copyright (c) 2013, 2015 Olli Vanhoja <olli.vanhoja@cs.helsinki.fi>
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

/**
 * @addtogroup libc
 * @{
 */

#ifndef STAT_H
#define STAT_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/types/_timespec.h>

struct stat {
    dev_t     st_dev;       /*!< ID of device containing file. */
    ino_t     st_ino;       /*!< File serial number */
    mode_t    st_mode;      /*!< Mode of file. */
    nlink_t   st_nlink;     /*!< Number of links to the file */
    uid_t     st_uid;       /*!< User ID of file. */
    gid_t     st_gid;       /*!< Group ID of file. */
    dev_t     st_rdev;      /*!< Device ID (if file is character or block
                             *   special). */
    off_t     st_size;      /*!< File size in bytes (if file is a regular file).
                             */
    struct timespec st_atim;      /*!< Time of last access.
                                   *   atime is updated when the file is opened.
                                   */
#define st_atime st_atim.tv_sec
    struct timespec st_mtim;      /*!< Time of last data modification.
                                   *   mtime is updated whenever the file is
                                   *   written.
                                   */
#define st_mtime st_mtim.tv_sec
    struct timespec st_ctim;      /*!< Time of last status change.
                                   *   ctime is updated when the file attributes
                                   *   are changed, when the file is moved or
                                   *   when the file is written. ctime is
                                   *   different from mtime if the file
                                   *   attributes are changed and thus only
                                   *   ctime get updated.
                                   */
#define st_ctime st_ctim.tv_sec
    struct timespec st_birthtime; /*!< Time file created. */
    fflags_t  st_flags;    /*!< User defined flags for file */
    blksize_t st_blksize;   /*!< A filesystem-specific preferred I/O block size
                                for this object.  In some filesystem types, this
                                may vary from file to file. */
    blkcnt_t  st_blocks;  /*!< Number of blocks allocated for this object. */
};

/* Symbolic names for the values of st_mode
 * -----------------------------------------
 * File type bits: */
#define S_IFMT      0170000 /*!< Bit mask for the file type bit fields. */
#define S_IFBLK     0060000 /*!< Block device (special). */
#define S_IFCHR     0020000 /*!< Character device (special). */
#define S_IFIFO     0010000 /*!< FIFO special. */
#define S_IFREG     0100000 /*!< Regular file. */
#define S_IFDIR     0040000 /*!< Directory. */
#define S_IFLNK     0120000 /*!< Symbolic link. */
#define S_IFSOCK    0140000 /*!< Socket. */
/* File mode bits: */
#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR) /*!< Mask for file owner permissions. */
#define S_IRUSR     0000400 /*!< Owner has read permission. */
#define S_IWUSR     0000200 /*!< Owner has write permission. */
#define S_IXUSR     0000100 /*!< Owner has execute permission. */
#define S_IRWXG (S_IRGRP | S_IWGRP | S_IXGRP) /*!< Mask for group permissions. */
#define S_IRGRP     0000040 /*!< Group has read permission. */
#define S_IWGRP     0000020 /*!< Group has write permission. */
#define S_IXGRP     0000010 /*!< Group has execute permission. */
#define S_IRWXO (S_IROTH | S_IWOTH | S_IXOTH) /*!< Mask for permissions for others. */
#define S_IROTH     0000004 /*!< Others have read permission. */
#define S_IWOTH     0000002 /*!< Others have write permission. */
#define S_IXOTH     0000001 /*!< Others have execute permission. */
#define S_ISUID     0004000 /*!< Set-user-ID bit. */
#define S_ISGID     0002000 /*!< Set-group-ID bit. */
#define S_ISVTX     0001000 /*!< On directories, restricted deletion flag. */

/** Test for a block special file. */
#define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
/** Test for a character special file. */
#define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
/** Test for a directory. */
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
/** Test for a pipe or FIFO special file. */
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
/** Test for a regular file. */
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
/** Test for a symbolic link. */
#define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
/** Test for a socked. */
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

/*
 * Definitions of flags stored in file flags word.
 * Super-user and owner changeable flags.
 */
#define UF_SETTABLE     0x0000ffff  /* Mask of owner changeable flags. */
#define UF_NODUMP       0x00000001  /* Do not dump file. */
#define UF_IMMUTABLE    0x00000002  /* File may not be changed. */
#define UF_APPEND       0x00000004  /* Writes to file may only append. */
#define UF_OPAQUE       0x00000008  /* Directory is opaque wrt. union. */
#define UF_NOUNLINK     0x00000010  /* File may not be removed or renamed. */
#define UF_SYSTEM       0x00000080  /* Windows system file bit. */
#define UF_SPARSE       0x00000100  /* Sparse file. */
#define UF_OFFLINE      0x00000200  /* File is offline. */
#define UF_REPARSE      0x00000400  /* Windows reparse point file bit. */
#define UF_ARCHIVE      0x00000800  /* File needs to be archived. */
#define UF_READONLY     0x00001000  /* Windows readonly file bit. */
#define UF_HIDDEN       0x00008000  /* File is hidden. */

/*
 * Super-user changeable flags.
 */
#define SF_SETTABLE     0xffff0000  /*!< Mask of superuser changeable flags. */
#define SF_ARCHIVED     0x00010000  /*!< File is archived. */
#define SF_IMMUTABLE    0x00020000  /*!< File may not be changed. */
#define SF_APPEND       0x00040000  /*!< Writes to file may only append. */
#define SF_NOUNLINK     0x00100000  /*!< file may not be removed or renamed. */
#define SF_SNAPSHOT     0x00200000  /*!< Snapshot inode. */

#if defined(__SYSCALL_DEFS__) || defined(KERNEL_INTERNAL)
/** Arguments for SYSCALL_FS_STAT */
struct _fs_stat_args {
    int fd;
    const char * path;
    size_t path_len;
    struct stat * buf;
    unsigned flags;
};

/** Arguments for SYSCALL_FS_UTIMES */
struct _fs_utimes_args {
    int fd;
    struct timespec times[2];
};

/** Arguments for SYSCALL_FS_CHMOD */
struct _fs_chmod_args {
    int fd;
    mode_t mode;
};

/** Arguments for SYSCALL_FS_CHFLAGS  */
struct _fs_chflags_args {
    int fd;
    fflags_t flags;
};

/** Arguments for SYSCALL_FS_MKDIR */
struct _fs_mkdir_args {
    int fd;
    const char * path;
    size_t path_len;
    mode_t mode;
    unsigned atflags;
};

/** Arguments for SYSCALL_FS_RMDIR */
struct _fs_rmdir_args {
    const char * path;
    size_t path_len;
};

/** Arguments for SYSCALL_FS_UMASK */
struct _fs_umask_args {
    mode_t newumask;
    mode_t oldumask;
};
#endif

#ifndef KERNEL_INTERNAL
__BEGIN_DECLS
int chmod(const char * path, mode_t mode);
int fchmodat(int fd, const char * path, mode_t mode, int flag);
int fchmod(int fd, mode_t mode);

int chflags(const char * path, unsigned long flags);
int fchflags(int fd, unsigned long flags);

int fstat(int fildes, struct stat * buf);
int fstatat(int fd, const char * restrict path,
            struct stat * restrict buf, int flag);
int lstat(const char * restrict path, struct stat * restrict buf);
int stat(const char * restrict path, struct stat * restrict buf);

int futimens(int fd, const struct timespec times[2]);
int utimensat(int fd, const char * path, const struct timespec times[2],
              int flag);

int mkdir(const char *, mode_t);
mode_t umask(mode_t cmask);
/*
int mkfifo(const char *, mode_t);
int mkdirat(int fd, const char * path, mode_t mode);
int mknod(const char *, mode_t, dev_t);
*/

__END_DECLS
#endif /* !KERNEL_INTERNAL */

#endif /* STAT_H */

/**
 * @}
 */
