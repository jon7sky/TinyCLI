#include "tcli.h"

static const tcli_arg_def_t _arg_def_common[] =
{
    { .s = "--to-go",       .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 0,   .mutex_idx = 0, .required = 0 },
    { .s = "--name=",       .type = ARG_TYPE_OPTION_HAS_VALUE,  .opt_idx = 0,   .mutex_idx = 1, .required = 0 }
};

static const tcli_arg_def_t arg_def_make_burger[] =
{
    { .s = "--white",       .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 1,   .mutex_idx = 2, .required = 1 },
    { .s = "--wheat",       .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 2,   .mutex_idx = 2, .required = 1 },
    { .s = "--ketchup",     .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 3,   .mutex_idx = 3, .required = 0 },
    { .s = "--mustard",     .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 4,   .mutex_idx = 4, .required = 0 },
};

static const tcli_arg_def_t arg_def_make_fries[] =
{
    { .s = "--extra-salt",  .type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 1,   .mutex_idx = 2, .required = 0 },
};

static const tcli_arg_def_t arg_def_deliver_to[] =
{
    { .s = "",              .type = ARG_TYPE_POSITIONAL,        .opt_idx = 1,   .mutex_idx = 2, .required = 1 },
    { .s = "",              .type = ARG_TYPE_POSITIONAL,        .opt_idx = 2,   .mutex_idx = 3, .required = 0 },
};

static const tcli_arg_def_t arg_def_eat[] =
{
    { .s = "",              .type = ARG_TYPE_POSITIONAL_MULTI,  .opt_idx = 1,   .mutex_idx = 2, .required = 1 },
};

static const tcli_cmd_def_t tcli_cmd_def[] =
{
    { .s1_idx = 0,                  .arg_def = &_arg_def_common[0],         .arg_def_cnt = 2 },
    { .s1_idx = 2,  .s2_idx = 7,    .arg_def = &arg_def_make_burger[0],     .arg_def_cnt = 4 },
    { .s1_idx = 2,  .s2_idx = 14,   .arg_def = &arg_def_make_fries[0],      .arg_def_cnt = 1 },
    { .s1_idx = 20, .s2_idx = 28,   .arg_def = &arg_def_deliver_to[0],      .arg_def_cnt = 2 },
    { .s1_idx = 31,                 .arg_def = &arg_def_eat[0],             .arg_def_cnt = 1 }
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
    .string_tbl = &tcli_string_tbl[0]
};
