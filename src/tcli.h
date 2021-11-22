#ifndef TCLI_H
#define TCLI_H

#include <stdint.h>
#include <string.h>
#include "tcli_def.h"

enum
{
    TCLI_OK                                     = 0,
    TCLI_ERROR_COMMAND_NOT_FOUND                = -1,
    TCLI_ERROR_OPTION_CONFLICT                  = -2,
    TCLI_ERROR_REQUIRED_OPTION_NOT_PROVIDED     = -3,
    TCLI_ERROR_COMMAND_NOT_SUPPORTED            = -4,
    TCLI_ERROR_UNKNOWN_OPTION                   = -5,
    TCLI_ERROR_AMBIGUOUS_COMMAND                = -6,
    TCLI_INTERNAL_ERROR                         = -100
};

enum
{
	ARG_TYPE_OPTION_BOOL = 0,
	ARG_TYPE_OPTION_HAS_VALUE,
    ARG_TYPE_POSITIONAL,
    ARG_TYPE_POSITIONAL_MULTI
};

typedef struct
{
    const char *s;
	int type;
	int opt_idx;
	int mutex_idx;
	int required;
} tcli_arg_def_t;

typedef struct
{
    int s1_idx;
    int s2_idx;
    int arg_def_idx;
    int arg_def_cnt;
} tcli_cmd_def_t;

typedef struct
{
    const tcli_cmd_def_t *cmd_def;
    const tcli_arg_def_t *arg_def;
    const char *string_tbl;
} tcli_def_t;

extern const tcli_def_t tcli_def;

int tcli_parse(char *buf, const tcli_def_t *tcli_def, tcli_args_t *args);
int tcli_cmd_handle(char *buf);
const char *next_arg(const char *arg);

#endif // TCLI_H
