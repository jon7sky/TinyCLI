#ifndef TCLI_H
#define TCLI_H

#include <stdint.h>
#include "tcli_def.h"

enum
{
    TCLI_OK                                     = 0,
    TCLI_ERROR_COMMAND_NOT_FOUND                = -1,
    TCLI_ERROR_OPTION_CONFLICT                  = -2,
    TCLI_ERROR_REQUIRED_OPTION_NOT_PROVIDED     = -3,
    TCLI_ERROR_COMMAND_NOT_SUPPORTED            = -4,
    TCLI_ERROR_UNKNOWN_OPTION                   = -5,
    TCLI_INTERNAL_ERROR                         = -6
};

enum
{
	ARG_TYPE_OPTION_BOOL = 0,
	ARG_TYPE_OPTION_HAS_VALUE
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
    const char *s;
    int slen;
    const tcli_arg_def_t *arg_def;
    int arg_def_cnt;
} tcli_cmd_def_t;

typedef struct
{
    const tcli_cmd_def_t *cmd_def;
} tcli_def_t;

extern const tcli_def_t tcli_def;

int tcli_parse(char *buf, const tcli_def_t *tcli_def, tcli_args_t *args);
int tcli_cmd_handle(char *buf);

#endif // TCLI_H
