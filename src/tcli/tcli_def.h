#ifndef TCLI_DEF_H
#define TCLI_DEF_H

#define TCLI_USE_HASH_FOR_CMDS 1

#include <stdint.h>

#define TCLI_USAGE_TEXT \
    "make burger [options]  (-w,--white | -h,--wheat)\n" \
    "    -k, --ketchup       Add ketchup\n" \
    "    -m, --mustard       Add Mustard\n" \
    "make fries [-l,--light-salt]\n" \
    "make biscuits [-g,--gravy]\n" \
    "make grits [-b,--butter -s,--sugar]\n" \
    "deliver to <name> [<address>]\n" \
    "eat <food>...\n" \
    "turn grill (on | off)\n" \
    "help\n" \
    ""

enum
{
    CMD_ID_NONE = 0,
    CMD_ID_make_burger,
    CMD_ID_make_fries,
    CMD_ID_make_biscuits,
    CMD_ID_make_grits,
    CMD_ID_deliver_to,
    CMD_ID_eat,
    CMD_ID_turn_grill,
    CMD_ID_help,
    CMD_ID_CNT
};

typedef uint32_t bool_options_t;

typedef struct
{
    bool_options_t white:1;
    bool_options_t wheat:1;
    bool_options_t ketchup:1;
    bool_options_t mustard:1;
    bool_options_t _pad_:28;
} tcli_args_make_burger_t;

typedef struct
{
    bool_options_t light_salt:1;
    bool_options_t _pad_:31;
} tcli_args_make_fries_t;

typedef struct
{
    bool_options_t gravy:1;
    bool_options_t _pad_:31;
} tcli_args_make_biscuits_t;

typedef struct
{
    bool_options_t butter:1;
    bool_options_t sugar:1;
    bool_options_t _pad_:30;
} tcli_args_make_grits_t;

typedef struct
{
    bool_options_t _pad_:32;
    char *name;
    char *address;
} tcli_args_deliver_to_t;

typedef struct
{
    bool_options_t _pad_:32;
    char *food;
} tcli_args_eat_t;

typedef struct
{
    bool_options_t on:1;
    bool_options_t off:1;
    bool_options_t _pad_:30;
} tcli_args_turn_grill_t;

typedef struct
{
    bool_options_t _pad_:32;
} tcli_args_help_t;

typedef struct
{
    bool_options_t bools;
    char *vals[2];
} tcli_args_generic_t;

typedef union
{
    tcli_args_generic_t generic;
    tcli_args_make_burger_t make_burger;
    tcli_args_make_fries_t make_fries;
    tcli_args_make_biscuits_t make_biscuits;
    tcli_args_make_grits_t make_grits;
    tcli_args_deliver_to_t deliver_to;
    tcli_args_eat_t eat;
    tcli_args_turn_grill_t turn_grill;
    tcli_args_help_t help;
} tcli_args_t;

int tcli_cmd_handle_make_burger(tcli_args_make_burger_t *args);
int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args);
int tcli_cmd_handle_make_biscuits(tcli_args_make_biscuits_t *args);
int tcli_cmd_handle_make_grits(tcli_args_make_grits_t *args);
int tcli_cmd_handle_deliver_to(tcli_args_deliver_to_t *args);
int tcli_cmd_handle_eat(tcli_args_eat_t *args);
int tcli_cmd_handle_turn_grill(tcli_args_turn_grill_t *args);
int tcli_cmd_handle_help(tcli_args_help_t *args);

#endif
