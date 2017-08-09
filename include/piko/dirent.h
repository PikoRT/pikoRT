#ifndef _LIBPIKO_DIRENT_H
#define _LIBPIKO_DIRENT_H

#include <sys/types.h>

#define NAME_MAX 32  // FIXME: Include <limits.h>

typedef void DIR;

struct dirent {
    ino_t d_ino;           /* inode number */
    char d_name[NAME_MAX]; /* filename */
};

DIR *opendir(const char *dirname);

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

int closedir(DIR *dirp);

#endif /* !_LIBPIKO_DIRENT_H */
