#include <sys/types.h>
#include <kernel/fs.h>
#include <drivers/mtd/mtd.h>

/* romfs depends on mtdchar */

static int mtdchar_open(struct inode *inode, struct file *file)
{
    file->f_private = inode->i_private;

    return 0;
}

static ssize_t mtdchar_read(struct file *file,
                            char *buf,
                            size_t count,
                            off_t offset)
{
    size_t retlen;
    struct mtd_info *mtd = file->f_private;

    if (mtd_read(mtd, offset, count, &retlen, (unsigned char *) buf) < 0)
        return -1;

    return retlen;
}

static ssize_t mtdchar_write(struct file *file,
                             const char *buf,
                             size_t count,
                             off_t *offset)
{
    size_t retlen;
    struct mtd_info *mtd = file->f_private;

    if (mtd_write(mtd, *offset, count, &retlen, (const unsigned char *) buf) <
        0)
        return -1;

    return retlen;
}

const struct file_operations mtdchar_fops = {
    .open = mtdchar_open,
    .read = mtdchar_read,
    .write = mtdchar_write,
};
