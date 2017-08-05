#include <stddef.h>
#include <drivers/mtd/mtd.h>

void mtd_erase_callback(struct erase_info *instr)
{
	if (instr->callback)
		instr->callback(instr);
}

int mtd_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	if ((instr->addr >= mtd->size) || (instr->len > mtd->size - instr->addr))
		return -EINVAL;
	if (!(mtd->flags & MTD_WRITEABLE))
		return -EROFS;
	instr->fail_addr = MTD_FAIL_ADDR_UNKNOWN;
	if (!instr->len) {
		instr->state = MTD_ERASE_DONE;
		mtd_erase_callback(instr);
		return 0;
	}

	return mtd->mtd_erase(mtd, instr);
}

int mtd_point(struct mtd_info *mtd,
              off_t from,
              size_t len,
              size_t *retlen,
              void **virt /* , resource_size_t *phys */)
{
	*retlen = 0;
	*virt = NULL;
	if (!mtd->mtd_point)
		return -1;  // return -EOPNOTSUPP;
	if ((from < 0) || ((unsigned long) from >= mtd->size) ||
	    (len > mtd->size - from))
		return -EINVAL;
	if (!len)
		return 0;

	return mtd->mtd_point(mtd, from, len, retlen, virt /* , phys */);
}

int mtd_read(struct mtd_info *mtd,
             off_t from,
             size_t len,
             size_t *retlen,
             unsigned char *buf)
{
	*retlen = 0;
	if ((from < 0) || ((unsigned long) from >= mtd->size) ||
	    (len > mtd->size - from))
		return -EINVAL;
	if (!len)
		return 0;

	return mtd->mtd_read(mtd, from, len, retlen, buf);
}

int mtd_write(struct mtd_info *mtd,
              off_t to,
              size_t len,
              size_t *retlen,
              const unsigned char *buf)
{
	*retlen = 0;
	if ((to < 0) || ((unsigned long) to >= mtd->size) || (len > mtd->size - to))
		return -EINVAL;
	if (!len)
		return 0;

	return mtd->mtd_write(mtd, to, len, retlen, buf);
}
