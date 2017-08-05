#include <string.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>

int mem_open(__unused struct inode *inode, __unused struct file *file)
{
	return 0;
}

ssize_t write_mem(__unused struct file *file,
                  const char *buf,
                  size_t count,
                  off_t *offset)
{
	memcpy((void *) offset, buf, count);

	return count;
}

ssize_t read_mem(__unused struct file *file,
                 char *buf,
                 size_t count,
                 off_t offset)
{
	memcpy(buf, (void *) offset, count);

	return count;
}

ssize_t write_null(__unused struct file *file,
                   __unused const char *buf,
                   size_t count,
                   __unused off_t *offset)
{
	return count;
}

ssize_t read_null(__unused struct file *file,
                  __unused char *buf,
                  __unused size_t count,
                  __unused off_t offset)
{
	return 0;
}

ssize_t read_zero(__unused struct file *file,
                  char *buf,
                  size_t count,
                  __unused off_t offset)
{
	memset(buf, 0, count);

	return count;
}

static const struct file_operations mem_fops = {
	.open = mem_open,
	.read = read_mem,
	.write = write_mem,
};

static const struct file_operations null_fops = {
	.open = mem_open,
	.read = read_null,
	.write = write_null,
};

static const struct file_operations zero_fops = {
	.open = mem_open,
	.read = read_zero,
	.write = write_null,
};

extern const struct file_operations random_fops;
extern const struct inode_operations tmpfs_iops;

static struct inode memdev_inodes[] = {
	{
		/* /dev/mem */
		.i_ino = 101,
		.i_op = &tmpfs_iops,
		.i_fop = &mem_fops,
	},
	{
		/* /dev/null */
		.i_ino = 102,
		.i_op = &tmpfs_iops,
		.i_fop = &null_fops,
	},
	{
		/* /dev/zero */
		.i_ino = 103,
		.i_op = &tmpfs_iops,
		.i_fop = &zero_fops,
	},
	{
		/* /dev/random */
		.i_ino = 104,
		.i_op = &tmpfs_iops,
		.i_fop = &random_fops,
	},
};

void memdev_init(void)
{
	struct dentry dentry;
	const char *names[] = {"mem", "null", "zero", "random"};

	for (int i = 0; i < 4; i++) {
		printk("Creating /dev/%s\n", names[i]);
		dentry.d_inode = &memdev_inodes[i], strcpy(dentry.d_name, names[i]);
		vfs_link(0, dev_inode(), &dentry);
	}
}
