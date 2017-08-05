#ifndef _SYSTEM_SH_H
#define _SYSTEM_SH_H

#define ARG_COUNT_MAX 8
#define BUF_LINE_LEN 128

enum ascii_control_char {
	ASCII_NULL = 000,
	ASCII_BACKSPACE = 010,
	ASCII_CARRIAGE_RETURN = 015,
	ASCII_ESCAPE = 033,
	ASCII_DELETE = 0177,
};

struct shell_cmd {
    char *name;
    int (*func)(int argc, char *argv[]);
};

#define SHELLCMD_REGIST(_name, _func) \
    static struct shell_cmd shell_##_name \
    __attribute__((section(".shell_cmd"), aligned(sizeof(long)), used)) = { \
        .name = #_name, \
        .func = _func }

#endif /* !_SYSTEM_SH_H */
