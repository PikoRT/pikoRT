#include <stddef.h>
#include <stdlib.h>
#include <kernel/types.h>
#include <kernel/irq.h>

#define IRQ_MAX 64

irq_handler_t irq_handler[IRQ_MAX];

// clang-format off
static struct irq_desc irq_desc[NR_IRQS] = {
    [0 ... NR_IRQS - 1] = {
        .irq_data = {.state = 0x0},
        .action = NULL,
	.status = 0x0,
	}
};
// clang-format on

static struct irq_desc *irq_to_desc(unsigned int irq)
{
    if (irq < NR_IRQS)
        return irq_desc + irq;
    return NULL;
}

int request_irq(unsigned int irq, irq_handler_t hdlr)
{
    struct irqaction *action;
    struct irq_desc *desc;

    desc = irq_to_desc(irq);

    if (desc && (desc->status & IRQ_NOREQUEST)) {
        action = (struct irqaction *) malloc(sizeof(struct irqaction));
        if (!action) {
            // WARN_ON(!action, "malloc failed\n");
            goto fail;
        }

        action->irq = irq;
        action->handler = hdlr;

        /* install to irq_desc */
        desc->action = action;

        irq_handler[irq] = hdlr;

        return 0;
    } else
    fail:
        return -1;
}

int free_irq(unsigned int irq)
{
    struct irq_desc *desc;

    desc = irq_to_desc(irq);

    if (desc && !(desc->status & IRQ_NOREQUEST)) {
        free(desc->action);
        desc->status |= IRQ_NOREQUEST;
        irq_handler[irq] = NULL;

        return 0;
    }

    return -1;
}

void early_irq_init(void)
{
    for (int irq = 0; irq < NR_IRQS; irq++)
        irq_desc[irq].irq_data.state = IRQD_IRQ_DISABLED;
}

void init_IRQ(void)
{
    for (int irq = 0; irq < NR_IRQS; irq++)
        irq_desc[irq].status = IRQ_NOREQUEST;
}
