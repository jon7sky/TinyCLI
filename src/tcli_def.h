#ifndef TCLI_DEF_H
#define TCLI_DEF_H

#include <stdint.h>

enum
{
    CMD_ID_NONE = 0,
    CMD_ID_make_burger,
    CMD_ID_make_fries,
    CMD_ID_CNT
};

typedef uint32_t bool_options_t;

typedef struct
{
    bool_options_t bools;
    char *args[10];
} tcli_args_generic_t;

typedef struct
{
    bool_options_t to_go:1;
    bool_options_t white:1;
    bool_options_t wheat:1;
    bool_options_t ketchup:1;
    bool_options_t mustard:1;
    char *name;
} tcli_args_make_burger_t;

typedef struct
{
    bool_options_t to_go:1;
    bool_options_t extra_salt:1;
    char *name;
} tcli_args_make_fries_t;

typedef union
{
    tcli_args_generic_t         generic;
    tcli_args_make_burger_t     make_burger;
    tcli_args_make_fries_t      make_fries;
} tcli_args_t;


int tcli_cmd_handle_make_burger(tcli_args_make_burger_t *args);
int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args);

#endif // TCLI_DEF_H
