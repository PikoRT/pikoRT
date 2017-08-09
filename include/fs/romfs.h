#ifndef _KERNEL_FS_ROMFS_H
#define _KERNEL_FS_ROMFS_H

#include <kernel/types.h>

#include <drivers/mtd/mtd.h>

#define ROMFS_FILETYPE_MASK 0x7

#define ROMFS_FILETYPE_HARD 0
#define ROMFS_FILETYPE_DIR 1
#define ROMFS_FILETYPE_REG 2
#define ROMFS_FILETYPE_LNK 3
#define ROMFS_FILETYPE_BLK 4
#define ROMFS_FILETYPE_CHR 5
#define ROMFS_FILETYPE_SOCKET 6
#define ROMFS_FILETYPE_FIFO 7

struct romfs_superblock {
    __u8 magic_number[8];
    __u32 full_size;
    __u32 checksum;
    char volume_name[0];
};

struct romfs_inode {
    __u32 next_filehdr;
    __u32 spec_info;
    __u32 size;
    __u32 checksum;
    char file_name[0];
};

#define ROMFS_SUPER_BLOCK(sb)                    \
    ({                                           \
        struct mtd_info *mtd = (sb)->s_private;  \
        struct romfs_superblock *rs = mtd->priv; \
        rs;                                      \
    })

#define ROMFS_INODE(rs, offset)                               \
    ({                                                        \
        __u32 addr = (__u32)(rs) + (__u32)(offset);           \
        struct romfs_inode *ri = (struct romfs_inode *) addr; \
        ri;                                                   \
    })

int romfs_mount(const char *source,
                const char *target,
                const char *filesystemtype,
                unsigned long mountflags,
                const void *data);

#endif /* !_KERNEL_FS_ROMFS_H */
