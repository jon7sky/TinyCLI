#include "tcli.h"

static const char tcli_string_tbl[] =
    "*\000"                         // 0
    "make\000"                      // 2
    "burger\000"                    // 7
    "fries\000"                     // 14
    "biscuits\000"                  // 20
    "deliver\000"                   // 29
    "to\000"                        // 37
    "eat\000"                       // 40
    "turn\000"                      // 44
    "grill\000"                     // 49
    "help\000"                      // 55
    ;

#define H(x) ((x) & 0x3fffff)

static const tcli_ca_def_t tcli_ca_def[] =
{
    // make burger
    { .cmd_def.s1_idx =   2, .cmd_def.s2_idx =   7, .cmd_def.arg_def_cnt =  6, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = 'w', .arg_def.hash = H(0x763a12d8), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 1 }, // white
    { .arg_def.short_char = 'h', .arg_def.hash = H(0xd4cc95ca), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // wheat
    { .arg_def.short_char = 'k', .arg_def.hash = H(0x5a31f5db), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // ketchup
    { .arg_def.short_char = 'm', .arg_def.hash = H(0xd091bf7b), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // mustard
    { .arg_def.short_char = 'g', .arg_def.hash = H(0xe5d842ab), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // to_go
    { .arg_def.short_char = 'n', .arg_def.hash = H(0xc4700b0c), .arg_def.has_val = 1, .arg_def.mutex = 1, .arg_def.required = 0 }, // name

    // make fries
    { .cmd_def.s1_idx =   2, .cmd_def.s2_idx =  14, .cmd_def.arg_def_cnt =  1, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = 'l', .arg_def.hash = H(0xcb9d1424), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // light_salt

    // make biscuits
    { .cmd_def.s1_idx =   2, .cmd_def.s2_idx =  20, .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },

    // deliver to
    { .cmd_def.s1_idx =  29, .cmd_def.s2_idx =  37, .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  2, .cmd_def.pos_req =  1, .cmd_def.pos_multi = 0 },

    // eat
    { .cmd_def.s1_idx =  40, .cmd_def.s2_idx =   0, .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  1, .cmd_def.pos_req =  1, .cmd_def.pos_multi = 1 },

    // turn grill
    { .cmd_def.s1_idx =  44, .cmd_def.s2_idx =  49, .cmd_def.arg_def_cnt =  2, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = ' ', .arg_def.hash = H(0x61342fd0), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 1 }, // on
    { .arg_def.short_char = ' ', .arg_def.hash = H(0xab3a8a0a), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // off

    // help
    { .cmd_def.s1_idx =  55, .cmd_def.s2_idx =   0, .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },

    // end of list
    { .cmd_def.s1_idx =  0, .cmd_def.s2_idx =  0, .cmd_def.arg_def_cnt = 0, .cmd_def.pos_cnt = 0, .cmd_def.pos_req = 0, .cmd_def.pos_multi = 0 },
};

const tcli_def_t tcli_def =
{
    .ca_def = &tcli_ca_def[0],
    .cmd_string_tbl = &tcli_string_tbl[0],
};
