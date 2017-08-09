#ifndef _DRIVERS_MTD_MTD_H
#define _DRIVERS_MTD_MTD_H

#include <sys/types.h>

#include <kernel/errno-base.h>
#include <kernel/types.h>

#define MTD_ERASE_PENDING (1 << 0)
#define MTD_ERASING (1 << 1)
#define MTD_ERASE_SUSPEND (1 << 2)
#define MTD_ERASE_DONE (1 << 3)
#define MTD_ERASE_FAILED (1 << 4)

#define MTD_FAIL_ADDR_UNKNOWN -1l

#define MTD_WRITEABLE 0x400 /* Device is writeable */

struct erase_info {
    /* struct mtd_info *mtd; */
    u32 addr;
    u32 len;
    u32 fail_addr;
    /* u_long time; */
    /* u_long retries; */
    /* unsigned dev; */
    /* unsigned cell; */
    void (*callback)(struct erase_info *self);
    unsigned long priv;
    unsigned char state;
    struct erase_info *next;
};

struct mtd_info {
    unsigned char type;
    u32 flags;
    unsigned long size;
    unsigned long erasesize;
    unsigned long writesize;

    const char *name;  // can this be a hash?

    int (*mtd_erase)(struct mtd_info *mtd, struct erase_info *instr);
    int (*mtd_point)(struct mtd_info *mtd,
                     off_t from,
                     size_t len,
                     size_t *retlen,
                     void **virt /* , resource_size_t *phys */);  // virt -> at
    int (*mtd_unpoint)(struct mtd_info *mtd, off_t from, size_t len);
    int (*mtd_read)(struct mtd_info *mtd,
                    off_t from,
                    size_t len,
                    size_t *retlen,
                    void *buf);
    int (*mtd_write)(struct mtd_info *mtd,
                     off_t to,
                     size_t len,
                     size_t *retlen,
                     const void *buf);

    /* struct device *dev; */
    void *priv;  // can be the physical address of the flash device
};

int mtd_erase(struct mtd_info *mtd, struct erase_info *instr);
int mtd_point(struct mtd_info *mtd,
              off_t from,
              size_t len,
              size_t *retlen,
              void **virt /* , resource_size_t *phys */);
int mtd_read(struct mtd_info *mtd,
             off_t from,
             size_t len,
             size_t *retlen,
             unsigned char *buf);
int mtd_write(struct mtd_info *mtd,
              off_t to,
              size_t len,
              size_t *retlen,
              const unsigned char *buf);

void mtd_erase_callback(struct erase_info *instr);

int mtdram_init_device(struct mtd_info *mtd,
                       void *mapped_address,
                       unsigned long size,
                       const char *name);
void mtdram_init(void);

#endif /* !_DRIVERS_MTD_MTD_H */
