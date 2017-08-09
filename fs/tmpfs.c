#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

#include "linux/list.h"

struct dirlist {
    struct inode *inode;
    char name[NAME_MAX];
    struct list_head list;
};

int tmpfs_mkdir(struct inode *dir, struct dentry *dentry, int mode)
{
    dir->i_mode = mode | S_IFDIR;
    dentry->d_inode = dir;

    struct list_head *dirlist = (struct list_head *) dir->i_private;
    INIT_LIST_HEAD(dirlist);

    return 0;
}

int tmpfs_link(__unused struct dentry *old_dentry,
               struct inode *dir,
               struct dentry *dentry)
{
    struct dirlist *new = malloc(sizeof(struct dirlist));
    if (!new)
        return -1;
    new->inode = dentry->d_inode;
    strncpy(new->name, dentry->d_name, NAME_MAX);

    struct list_head *dirlist = (struct list_head *) dir->i_private;
    list_add_tail(&new->list, dirlist);
    dir->i_size++;

    return 0;
}

int tmpfs_iterate(struct file *file, struct dir_context *ctx)
{
    int res = -1;
    struct inode *inode = file->f_dentry->d_inode;

    if (file->f_pos == inode->i_size + 2)
        return -1;
    switch (file->f_pos) {
    case 0:
        res = dir_emit_dot(file, ctx);
        break;
    case 1:
        res = dir_emit_dotdot(file, ctx);
        break;
    default:;
        struct dirlist *dirlist;
        struct list_head *head = (struct list_head *) inode->i_private;
        int i = 2;
        list_for_each_entry(dirlist, head, list)
        {
            if (i++ == file->f_pos) {
                res = dir_emit(ctx, dirlist->name, strlen(dirlist->name),
                               dirlist->inode->i_ino, 0);
                break;
            }
        }
    }
    file->f_pos++;

    return res;
}

const struct dentry_operations tmpfs_dops;

struct dentry *tmpfs_lookup(struct inode *dir, struct dentry *target)
{
    struct list_head *head = (struct list_head *) dir->i_private;
    struct dirlist *dirlist;

    list_for_each_entry(dirlist, head, list)
    {
        if (!strcmp(target->d_name, dirlist->name)) {
            target->d_inode = dirlist->inode;
            target->d_op = &tmpfs_dops;

            return target;
        }
    }

    return NULL;
}

int tmpfs_delete(struct dentry *dentry)
{
    free(dentry);

    return 0;
}

const struct inode_operations tmpfs_iops = {
    .lookup = tmpfs_lookup,
    .link = tmpfs_link,
};

const struct file_operations tmpfs_fops = {
    .iterate = tmpfs_iterate,
};

const struct dentry_operations tmpfs_dops = {
    .delete = tmpfs_delete,
};

static struct inode tmpfs_inodes[] = {
    {
        /* /      - the root directory */
        .i_ino = 1,
        .i_op = &tmpfs_iops,
        .i_fop = &tmpfs_fops,
        .i_mode = S_IFDIR,
        .i_size = 2,
        .i_private = &((struct list_head){}),
    },
    {
        /* /dev   - essential device files */
        .i_ino = 2,
        .i_op = &tmpfs_iops,
        .i_fop = &tmpfs_fops,
        .i_mode = S_IFDIR,
        .i_private = &((struct list_head){}),
    },
    {
        /* /proc  - process and kernel information as files */
        .i_ino = 3,
        .i_op = &tmpfs_iops,
        .i_fop = &tmpfs_fops,
        .i_mode = S_IFDIR,
        .i_private = &((struct list_head){}),
    },
};

struct inode *root_inode(void)
{
    return &tmpfs_inodes[0];
}

struct inode *dev_inode(void)
{
    return &tmpfs_inodes[1];
}

struct inode *proc_inode(void)
{
    return &tmpfs_inodes[2];
}

struct dentry *root_dentry(void)
{
    static struct dentry dentry = {
        .d_name = "/",
        .d_inode = &tmpfs_inodes[0],
        .d_parent = &dentry,
        .d_op = &tmpfs_dops,
    };

    return &dentry;
}

struct inode *init_tmpfs_inode(struct inode *inode)
{
    static ino_t ino = 9000;

    inode->i_ino = ino++;
    inode->i_op = &tmpfs_iops;

    return inode;
}

void tmpfs_init(void)
{
    static struct dirlist entries[] = {
        {
            .inode = &tmpfs_inodes[1], .name = "dev",
        },
        {
            .inode = &tmpfs_inodes[2], .name = "proc",
        },
    };

    struct list_head *rootdir = (struct list_head *) tmpfs_inodes[0].i_private;
    INIT_LIST_HEAD(rootdir);
    list_add_tail(&entries[0].list, rootdir);
    list_add_tail(&entries[1].list, rootdir);

    struct list_head *devdir = (struct list_head *) tmpfs_inodes[1].i_private;
    INIT_LIST_HEAD(devdir);

    struct list_head *procdir = (struct list_head *) tmpfs_inodes[2].i_private;
    INIT_LIST_HEAD(procdir);
}
