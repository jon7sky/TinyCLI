#include "tcli.h"

static const tcli_arg_def_t tcli_arg_def[] =
{
    { .s = " ",             .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 0,   .mutex_idx = 0, .required = 0 },        // 0 - NULL

    { .s = "--to-go",       .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 0,   .mutex_idx = 0, .required = 0 },        // 1 - common
    { .s = "--name=",       .type = ARG_TYPE_OPTION_HAS_VALUE,  .opt_idx = 0,   .mutex_idx = 1, .required = 0 },        // 2

    { .s = "--white",       .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 1,   .mutex_idx = 2, .required = 1 },        // 3 - make burger
    { .s = "--wheat",       .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 2,   .mutex_idx = 2, .required = 1 },        // 4
    { .s = "--ketchup",     .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 3,   .mutex_idx = 3, .required = 0 },        // 5
    { .s = "--mustard",     .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 4,   .mutex_idx = 4, .required = 0 },        // 6

    { .s = "--extra-salt",  .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 1,   .mutex_idx = 2, .required = 0 },        // 7 - make fries

    { .s = "",              .type = ARG_TYPE_POSITIONAL,        .opt_idx = 1,   .mutex_idx = 2, .required = 1 },        // 8 - deliver to
    { .s = "",              .type = ARG_TYPE_POSITIONAL,        .opt_idx = 2,   .mutex_idx = 3, .required = 0 },        // 9

    { .s = "",              .type = ARG_TYPE_POSITIONAL_MULTI,  .opt_idx = 1,   .mutex_idx = 2, .required = 1 },        // 10 - eat
};

static const tcli_cmd_def_t tcli_cmd_def[] =
{
    { .s1_idx = 0,                  .arg_def_idx = 1,   .arg_def_cnt = 2 }, // Common
    { .s1_idx = 2,  .s2_idx = 7,    .arg_def_idx = 3,   .arg_def_cnt = 4 }, // make burger
    { .s1_idx = 2,  .s2_idx = 14,   .arg_def_idx = 7,   .arg_def_cnt = 1 }, // make fries
    { .s1_idx = 20, .s2_idx = 28,   .arg_def_idx = 8,   .arg_def_cnt = 2 }, // deliver to
    { .s1_idx = 31,                 .arg_def_idx = 10,  .arg_def_cnt = 1 }  // eat
};

const char tcli_string_tbl[] =
    " \000"         // 0
    "make\000"      // 2
    "burger\000"    // 7
    "fries\000"     // 14
    "deliver\000"   // 20
    "to\000"        // 28
    "eat\000";      // 31

const tcli_def_t tcli_def =
{
    .cmd_def = &tcli_cmd_def[0],
    .arg_def = &tcli_arg_def[0],
    .string_tbl = &tcli_string_tbl[0]
};
