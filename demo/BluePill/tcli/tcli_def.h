#ifndef TCLI_DEF_H
#define TCLI_DEF_H

#define TCLI_USE_HASH_FOR_CMDS 1

#include <stdint.h>

#define TCLI_USAGE_TEXT \
    "led (on | off)\n" \
    "bambi\n" \
    "help\n" \
    ""

enum
{
    CMD_ID_NONE = 0,
    CMD_ID_led,
    CMD_ID_bambi,
    CMD_ID_help,
    CMD_ID_CNT
};

typedef uint32_t bool_options_t;

typedef struct
{
    bool_options_t on:1;
    bool_options_t off:1;
    bool_options_t _pad_:30;
} tcli_args_led_t;

typedef struct
{
    bool_options_t _pad_:32;
} tcli_args_bambi_t;

typedef struct
{
    bool_options_t _pad_:32;
} tcli_args_help_t;

typedef struct
{
    bool_options_t bools;
    char *vals[0];
} tcli_args_generic_t;

typedef union
{
    tcli_args_generic_t generic;
    tcli_args_led_t led;
    tcli_args_bambi_t bambi;
    tcli_args_help_t help;
} tcli_args_t;

int tcli_cmd_handle_led(tcli_args_led_t *args);
int tcli_cmd_handle_bambi(tcli_args_bambi_t *args);
int tcli_cmd_handle_help(tcli_args_help_t *args);

#endif
