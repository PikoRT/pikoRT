#include <sys/types.h>
#include <kernel/fs.h>
#include <kernel/sched.h>
#include <kernel/serial.h>
#include <kernel/thread.h>

static void serialchar_callback(struct serial_info *serial)
{
    sched_enqueue(serial->owner);
}

static int serialchar_open(struct inode *inode, struct file *file)
{
    file->f_private = inode->i_private;
    struct serial_info *serial = file->f_private;
    CURRENT_THREAD_INFO(cur_thread);
    serial->owner = cur_thread;
    serial->ops->callback = serialchar_callback;

    return 0;
}

static ssize_t serialchar_read(struct file *file,
                               char *buf,
                               size_t count,
                               __unused off_t offset)
{
    size_t retlen;
    struct serial_info *serial = file->f_private;

    while (serial->rx_count < count) {
        CURRENT_THREAD_INFO(cur_thread);
        sched_dequeue(cur_thread);
        sched_elect(0);
    }
    if (count == 1)
        return serial_getc(serial, buf);
    if (serial_gets(serial, count, &retlen, buf) < 0)
        return -1;

    return retlen;
}

static ssize_t serialchar_write(struct file *file,
                                const char *buf,
                                size_t count,
                                __unused off_t *offset)
{
    size_t retlen;
    struct serial_info *serial = file->f_private;

    if (count == 1)
        return serial_putc(serial, *((char *) buf));
    if (serial_puts(serial, count, &retlen, buf) < 0)
        return -1;

    return retlen;
}

const struct file_operations serialchar_fops = {
    .open = serialchar_open,
    .read = serialchar_read,
    .write = serialchar_write,
};
