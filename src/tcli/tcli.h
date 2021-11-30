#ifndef TCLI_H
#define TCLI_H

#include <stdint.h>
#include <string.h>
#include "tcli_def.h"

enum
{
    TCLI_OK                                     = 0,
    TCLI_ERROR_COMMAND_NOT_FOUND                = -1,
    TCLI_ERROR_ARG_CONFLICT                     = -2,
    TCLI_ERROR_REQUIRED_ARG_NOT_PROVIDED        = -3,
    TCLI_ERROR_COMMAND_NOT_SUPPORTED            = -4,
    TCLI_ERROR_UNKNOWN_OPTION                   = -5,
    TCLI_ERROR_AMBIGUOUS_COMMAND                = -6,
    TCLI_ERROR_TOO_MANY_ARGS                    = -7,
    TCLI_INTERNAL_ERROR                         = -100
};

typedef struct
{
    uint32_t hash:22;
    uint32_t short_char:7;
    uint32_t has_val:1;
    uint32_t mutex:1;
    uint32_t required:1;
} tcli_arg_def_t;

typedef struct
{
#if TCLI_USE_HASH_FOR_CMDS
    uint32_t hash:20;
#else
    uint32_t s1_idx:10;     // Command string table size 1024 bytes
    uint32_t s2_idx:10;
#endif
    uint32_t arg_def_cnt:5; // 32 options per command
    uint32_t pos_cnt:3;     // 8 positional arguments
    uint32_t pos_req:3;
    uint32_t pos_multi:1;
} tcli_cmd_def_t;

typedef union
{
    tcli_arg_def_t arg_def;
    tcli_cmd_def_t cmd_def;
} tcli_ca_def_t;

typedef struct
{
    const tcli_ca_def_t *ca_def;
    const char *cmd_string_tbl;
} tcli_def_t;

extern const tcli_def_t tcli_def;

int tcli_parse(char *buf, const tcli_def_t *tcli_def, tcli_args_t *args);
int tcli_cmd_handle(char *buf);
const char *tcli_next_arg(const char *arg);
const char *tcli_error(int error);

#endif // TCLI_H
