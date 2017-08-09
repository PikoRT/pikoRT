#ifndef IF_GPIO_H
#define IF_GPIO_H

#define GPIO_DIR_OUT 0
#define GPIO_DIR_IN 1

#ifndef GPIO_COUNT
#define GPIO_COUNT 0
#endif

void __gpio_set(unsigned int offset, int value);
int __gpio_get(unsigned int offset);
int __gpio_direction_input(unsigned int offset);
int __gpio_direction_output(unsigned offset, int value);

static inline void gpio_set(unsigned int offset, int value)
{
    __gpio_set(offset, value);
}

static inline int gpio_get(unsigned int offset)
{
    return __gpio_get(offset);
}

static inline int gpio_direction_input(unsigned int offset)
{
    return __gpio_direction_input(offset);
}

static inline int gpio_direction_output(unsigned int offset, int value)
{
    return __gpio_direction_output(offset, value);
}

#endif /* !IF_GPIO_H */
