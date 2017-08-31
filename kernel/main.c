#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <sys/fcntl.h>

#include <kernel/fs.h>
#include <kernel/kernel.h>
#include <kernel/mm/page.h>
#include <kernel/mm/slab.h>
#include <kernel/sched.h>
#include <kernel/task.h>
#include <kernel/thread.h>
#include <kernel/compiler.h>
#include <kernel/softirq.h>

#include "platform.h"

extern char __early_stack_start__;
extern char __early_stack_end__;
extern char __text_start__;
extern char __text_end__;
extern char __rodata_start__;
extern char __rodata_end__;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __pgmem_start__;
extern char __pgmem_end__;
extern char __pgmem_size__;
extern char __heap_start__;
extern char __heap_end__;
extern char __heap_size__;

void __do_idle(void);
void *do_idle(void *);
void mtdram_init(void);
void __printk_init(void);
int minishell(void *options);
void memdev_init(void);
void kernel_heap_init(void *heap_start, size_t heap_size);
void early_irq_init(void);
void init_IRQ(void);

struct task_info idle_task;
struct task_info main_task;

void print_version(void)
{
    char buf[] = {0, 0};
    int fd = open("/proc/version", 0);

    while (read(fd, &buf, 1))
        printk("%s", buf);
    close(fd);
    printk("\n");
}

void __weak *main(__unused void *arg)
{
    print_version();
    minishell(NULL);

    return 0;
}

struct thread_info *thread_idle;

/* Cortex-M3/4 system initialization */
static void v7m_init(void)
{
    /* enable UsageFault, BusFault, MemManage faults */
    SCB->SHCSR |= (SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk |
                   SCB_SHCSR_MEMFAULTENA_Msk);

    /* Configure the System Control Register to ensure 8-byte stack
       alignment */
    SCB->CCR |= SCB_CCR_STKALIGN_Msk;

    // NVIC_SetPriority(DebugMonitor_IRQn, 0x0, 0);

    NVIC_SetPriority(MemoryManagement_IRQn, 0x1);
    NVIC_SetPriority(BusFault_IRQn, 0x1);
    NVIC_SetPriority(UsageFault_IRQn, 0x1);

    NVIC_SetPriority(SysTick_IRQn, 0x3);

    /* Priority 0xF - debug_uart */
    NVIC_SetPriority(SVCall_IRQn, 0xF);
    NVIC_SetPriority(PendSV_IRQn, 0xF);

    /* follow the architectural requirements */
    __DSB();
}

void print_linker_sections(void)
{
    printk("Memory map:\n");
    printk("  .text	  = %08x--%08x	%6d Bytes\n", &__text_start__,
           &__text_end__, &__text_end__ - &__text_start__);
    printk("  .rodata = %08x--%08x	%6d Bytes\n", &__rodata_start__,
           &__rodata_end__, &__rodata_end__ - &__rodata_start__);
    printk("  .data	  = %08x--%08x	%6d Bytes\n", &__data_start__,
           &__data_end__, &__data_end__ - &__data_start__);
    printk("  .bss	  = %08x--%08x	%6d Bytes\n", &__bss_start__, &__bss_end__,
           &__bss_end__ - &__bss_start__);
    printk("  .heap	  = %08x--%08x	%6d Bytes\n", &__heap_start__,
           &__heap_end__, &__heap_end__ - &__heap_start__);
    printk("  .pgmem  = %08x--%08x	%6d Bytes\n", &__pgmem_start__,
           &__pgmem_end__, &__pgmem_end__ - &__pgmem_start__);
}


struct thread_info *start_kernel(void)
{
    v7m_init();

    early_irq_init();
    init_IRQ();

    /* TODO: Early console */
    __printk_init();

    /* initialize the kernel's malloc */
    kernel_heap_init(&__heap_start__, (size_t) &__heap_size__);

    print_linker_sections();

    /* initialize the physical memory allocator */
    show_page_bitmap();  // init_pages();
    kmem_cache_init();

    /* initialize the scheduler internels */
    sched_init();
    /* select giving scheduling policy */
    sched_select(SCHED_CLASS_BITMAP);

    /* idle_thread is not added to the runqueue */
    task_init(&idle_task);
    thread_idle =
        thread_create(do_idle, NULL, THREAD_PRIV_SUPERVISOR, 1024, &idle_task);
    if (!thread_idle) {
        printk("[!] Could not create system idle thread.\n");
        return NULL;
    }
    printk("Created idle_thread at <%p>\n", thread_idle);

    /* The main_thread is the user's entry-point to the system.  It is not
     * added to the runqueue because it has been implicitly "elected" when
     * start_kernel() returns.    */
    task_init(&main_task);
    struct thread_info *thread_main =
        thread_create(main, NULL, THREAD_PRIV_USER, 1024, &main_task);
    if (!thread_main) {
        printk("[!] Could not create user main thread.\n");
        return NULL;
    }
    printk("Created main_thread at <%p> with priority=%d\n", thread_main,
           thread_main->ti_priority);

    /* Reclaim the early-stack physical memory.  In the current context, no
     * page allocation after this point are allowed.    */
    printk("Reclaim early stack's physical memory (%d Bytes, order=%d).\n",
           &__early_stack_start__ - &__early_stack_end__,
           size_to_page_order(2048));
    free_pages((unsigned long) &__early_stack_end__, size_to_page_order(2048));

    tmpfs_init();
    proc_init();
    memdev_init();
    mtdram_init(); /* create a test mtdram device */

    /* do the platform-specific inits */
    __platform_init();

    init_softirq();

    printk("Kernel bootstrap done.\n--\n");

    return thread_main;
}

void *do_idle(__unused void *arg)
{
    for (;;)
        __do_idle();
}
