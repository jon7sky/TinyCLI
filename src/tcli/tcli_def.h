#ifndef TCLI_DEF_H
#define TCLI_DEF_H

#include <stdint.h>

enum
{
    CMD_ID_NONE = 0,
    CMD_ID_make_burger,
    CMD_ID_make_biscuits,
    CMD_ID_make_fries,
    CMD_ID_deliver_to,
    CMD_ID_eat,
    CMD_ID_turn_grill,
    CMD_ID_CNT
};

typedef uint32_t bool_options_t;

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
    char *name;
} tcli_args_make_biscuits_t;

typedef struct
{
    bool_options_t to_go:1;
    bool_options_t light_salt:1;
    char *name;
} tcli_args_make_fries_t;

typedef struct
{
    bool_options_t to_go:1;
    char *name;
    char *person;
    char *address;
} tcli_args_deliver_to_t;

typedef struct
{
    bool_options_t to_go:1;
    char *name;
    char *food;
} tcli_args_eat_t;

typedef struct
{
    bool_options_t to_go:1;
    bool_options_t on:1;
    bool_options_t off:1;
    char *name;
} tcli_args_turn_grill_t;

typedef struct
{
    bool_options_t bools;
    char *args[3];
} tcli_args_generic_t;

typedef union
{
    tcli_args_generic_t generic;
    tcli_args_make_burger_t make_burger;
    tcli_args_make_biscuits_t make_biscuits;
    tcli_args_make_fries_t make_fries;
    tcli_args_deliver_to_t deliver_to;
    tcli_args_eat_t eat;
    tcli_args_turn_grill_t turn_grill;
} tcli_args_t;

int tcli_cmd_handle_make_burger(tcli_args_make_burger_t *args);
int tcli_cmd_handle_make_biscuits(tcli_args_make_biscuits_t *args);
int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args);
int tcli_cmd_handle_deliver_to(tcli_args_deliver_to_t *args);
int tcli_cmd_handle_eat(tcli_args_eat_t *args);
int tcli_cmd_handle_turn_grill(tcli_args_turn_grill_t *args);

#endif
