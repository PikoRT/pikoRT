#include <stdlib.h>
#include <string.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

#include "linux/list.h"

static int fillonedir(struct dir_context *ctx,
                      const char *name,
                      int namlen,
                      __unused off_t offset,
                      unsigned int ino,
                      __unused unsigned int d_type)
{
	struct readdir_callback *buf =
	    container_of(ctx, struct readdir_callback, ctx);
	struct piko_dirent *dirent = buf->dirent;

	dirent->d_ino = ino;
	strncpy(dirent->d_name, name, namlen);
	dirent->d_name[namlen] = '\0';

	return 0;
}

int sys_readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
	struct file *file = fd_to_file((int) dirp);
	struct readdir_callback buf = {
		.ctx = {.actor = fillonedir, .pos = 0},
		.dirent = (struct piko_dirent *) entry,
	};

	if (vfs_iterate(file, &buf.ctx))
		*result = NULL;
	else
		*result = entry;

	return 0;
}
