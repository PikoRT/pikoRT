#ifndef KERNEL_IRQ_H
#define KERNEL_IRQ_H

#define NR_IRQS 64

/* irq status */
enum {
    IRQ_NOREQUEST = 1,
};

/* irq data state(for per-chip) */
enum {
    IRQD_IRQ_DISABLED = 1,
    IRQD_INACTIVATED = 1 << 2,
    IRQD_ACTIVATED = 1 << 3,
    IRQD_PENDING = 1 << 4,
};

/**
 * struct irq_data - per chip data
 *
 * @state:  status information for irq chip
 * XXX: Thanks for CMSIS. We don't have to impl per chip function and data,
 *          but this concept should be remained due to more precious description
 *          of irqs and chips(HW).
 */
struct irq_data {
    unsigned int state;
};

struct irqaction;

/**
 * struct irq_desc - interrupt descriptor
 *
 * @action:		the irq action chain
 * @status:		status information
 * @irq_state:	per irq state information
 */
struct irq_desc {
    struct irq_data irq_data;
    struct irqaction *action;
    unsigned int status;
};

typedef void (*irq_handler_t)(void);
/**
 * struct irqaction - per interrupt action descriptor
 * @handler:	interrupt handler function
 * @irq:	interrupt number
 */
struct irqaction {
    irq_handler_t handler;
    unsigned int irq;
};

int request_irq(unsigned int irq, irq_handler_t hdlr);
int free_irq(unsigned int irq);

#endif /* !KERNEL_IRQ_H */
