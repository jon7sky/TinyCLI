#ifndef TCLI_H
#define TCLI_H

#include <stdint.h>

enum
{
    TCLI_ERROR_COMMAND_NOT_FOUND                = -1,
    TCLI_ERROR_OPTION_CONFLICT                  = -2,
    TCLI_ERROR_REQUIRED_OPTION_NOT_PROVIDED     = -3
};

enum
{
    CMD_ID_NONE = 0,
    CMD_ID_make_burger,
    CMD_ID_make_fries,
    CMD_ID_CNT
};

enum
{
	ARG_TYPE_OPTION_BOOL = 0,
	ARG_TYPE_OPTION_HAS_VALUE
};

typedef uint32_t bool_options_t;

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

typedef union
{
    struct
    {
        bool_options_t bools;
        char *args[10];
    } generic;
    struct
    {
        bool_options_t white:1;
        bool_options_t wheat:1;
        bool_options_t ketchup:1;
        bool_options_t mustard:1;
        char *name;
    } make_burger;
    struct
    {
        bool_options_t extra_salt:1;
    } make_fries;
} tcli_args_t;

int tcli_parse(char *buf, const tcli_def_t *tcli_def, tcli_args_t *args);

#endif // TCLI_H
