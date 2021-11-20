#ifndef TCLI_H
#define TCLI_H

#include <stdint.h>

enum
{
    CMD_ID_NONE = 0,
    CMD_ID_make_burger,
    CMD_ID_make_fries,
    CMD_ID_CNT
};

typedef struct
{
    const char *str;
} tcli_arg_def_t;

typedef struct
{
    const char *str;
} tcli_cmd_def_t;

typedef struct
{
    const struct tcli_cmd_def_t *cmd_def;
} tcli_def_t;

int tcli_parse(char *buf, const tcli_def_t *tcli_def);

#endif // TCLI_H
