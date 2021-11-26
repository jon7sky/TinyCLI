#include "tcli.h"

const char tcli_string_tbl[] =
        " \000"             // 0
        "make\000"          // 2
        "burger\000"        // 7
        "fries\000"         // 14
        "deliver\000"       // 20
        "to\000"            // 28
        "eat\000"           // 31
        "turn\000"          // 35
        "grill\000"         // 40
        "biscuits\000"      // 46
        ;

const char tcli_arg_string_tbl[] =
        " \000"             // 0
        "--to-go\000"       // 2
        "--name\000"        // 10
        "--white\000"       // 17
        "--wheat\000"       // 25
        "--ketchup\000"     // 33
        "--mustard\000"     // 43
        "--light-salt\000"  // 53
        "on\000"            // 66
        "off\000"           // 69
        ;

static const tcli_arg_def_t tcli_arg_def[] =
{
    { .short_char = 0,   .long_idx = 0,  .type = ARG_TYPE_OPTION_BOOL,       .mutex = 1, .required = 0 },        // 0 - NULL

    // common
    { .short_char = 'g', .long_idx = 2,  .type = ARG_TYPE_OPTION_BOOL,       .mutex = 1, .required = 0 },        // 1 - to-go
    { .short_char = 'n', .long_idx = 10, .type = ARG_TYPE_OPTION_HAS_VALUE,  .mutex = 1, .required = 0 },        // 2 - name

    // make burger
    { .short_char = 'g', .long_idx = 2,  .type = ARG_TYPE_OPTION_BOOL,       .mutex = 1, .required = 0 },        // 3 - to-go
    { .short_char = 'n', .long_idx = 10, .type = ARG_TYPE_OPTION_HAS_VALUE,  .mutex = 1, .required = 0 },        // 4 - name
    { .short_char = 'w', .long_idx = 17, .type = ARG_TYPE_OPTION_BOOL,       .mutex = 1, .required = 1 },        // 5 - white
    { .short_char = 'h', .long_idx = 25, .type = ARG_TYPE_OPTION_BOOL,       .mutex = 0, .required = 1 },        // 6 - wheat
    { .short_char = 'k', .long_idx = 33, .type = ARG_TYPE_OPTION_BOOL,       .mutex = 1, .required = 0 },        // 7 - ketchup
    { .short_char = 'm', .long_idx = 43, .type = ARG_TYPE_OPTION_BOOL,       .mutex = 1, .required = 0 },        // 8 - mustard

    // make fries
    { .short_char = 'l',  .long_idx = 53, .type = ARG_TYPE_OPTION_BOOL,      .mutex = 1, .required = 0 },        // 9 - light-salt

    // deliver to
    { .short_char = 0,   .long_idx = 0,  .type = ARG_TYPE_POSITIONAL,        .mutex = 1, .required = 1 },        // 10 (positional)
    { .short_char = 0,   .long_idx = 0,  .type = ARG_TYPE_POSITIONAL,        .mutex = 1, .required = 0 },        // 11 (positional)

    // eat
    { .short_char = 0,   .long_idx = 0,  .type = ARG_TYPE_POSITIONAL_MULTI,  .mutex = 1, .required = 1 },        // 12 (positional)

    // turn grill
    { .short_char = 0,   .long_idx = 66, .type = ARG_TYPE_OPTION_BOOL,       .mutex = 1, .required = 1 },        // 13 - on
    { .short_char = 0,   .long_idx = 69, .type = ARG_TYPE_OPTION_BOOL,       .mutex = 0, .required = 1 },        // 14 - off
};

static const tcli_cmd_def_t tcli_cmd_def[] =
{
    { .s1_idx = 0,                  .arg_def_idx = 1,   .arg_def_cnt = 2 }, // Common
    { .s1_idx = 2,  .s2_idx = 7,    .arg_def_idx = 3,   .arg_def_cnt = 6 }, // make burger
    { .s1_idx = 2,  .s2_idx = 46,   .arg_def_idx = 0,   .arg_def_cnt = 0 }, // make biscuits
    { .s1_idx = 2,  .s2_idx = 14,   .arg_def_idx = 9,   .arg_def_cnt = 1 }, // make fries
    { .s1_idx = 20, .s2_idx = 28,   .arg_def_idx = 10,  .arg_def_cnt = 2 }, // deliver to
    { .s1_idx = 31,                 .arg_def_idx = 12,  .arg_def_cnt = 1 }, // eat
    { .s1_idx = 35, .s2_idx = 40,   .arg_def_idx = 13,  .arg_def_cnt = 2 }  // turn grill
};

const tcli_def_t tcli_def =
{
    .cmd_def = &tcli_cmd_def[0],
    .arg_def = &tcli_arg_def[0],
    .cmd_string_tbl = &tcli_string_tbl[0],
    .arg_string_tbl = &tcli_arg_string_tbl[0]
};
