#define mp_hal_stdout_tx_strn_cooked(s, l) printk(s);
#define mp_hal_stdout_tx_str(s) printk(s);
#define mp_hal_set_interrupt_char(n) (0)
#define mp_hal_ticks_ms() (0)
