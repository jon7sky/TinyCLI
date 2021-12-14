#include "tcli.h"

#define HA(x) ((x) & 0x003fffff)
#define HC(x) ((x) & 0x000fffff)

static const tcli_ca_def_t tcli_ca_def[] =
{
    // make burger
    { .cmd_def.hash = HC(0x51e006ba), .cmd_def.arg_def_cnt =  4, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = 'w', .arg_def.hash = HA(0x763a12d8), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 1 }, // white
    { .arg_def.short_char = 'h', .arg_def.hash = HA(0xd4cc95ca), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // wheat
    { .arg_def.short_char = 'k', .arg_def.hash = HA(0x5a31f5db), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // ketchup
    { .arg_def.short_char = 'm', .arg_def.hash = HA(0xd091bf7b), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // mustard

    // make fries
    { .cmd_def.hash = HC(0x0b401aae), .cmd_def.arg_def_cnt =  1, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = 'l', .arg_def.hash = HA(0xcb9d1424), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // light_salt

    // make biscuits
    { .cmd_def.hash = HC(0x27d6741b), .cmd_def.arg_def_cnt =  1, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = 'g', .arg_def.hash = HA(0xd4901648), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // gravy

    // make grits
    { .cmd_def.hash = HC(0x512f0000), .cmd_def.arg_def_cnt =  2, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = 'b', .arg_def.hash = HA(0xa17f1011), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // butter
    { .arg_def.short_char = 's', .arg_def.hash = HA(0x4b5231af), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 0 }, // sugar

    // deliver to
    { .cmd_def.hash = HC(0x4b82c5b3), .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  2, .cmd_def.pos_req =  1, .cmd_def.pos_multi = 0 },

    // eat
    { .cmd_def.hash = HC(0x6c6e2525), .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  1, .cmd_def.pos_req =  1, .cmd_def.pos_multi = 1 },

    // turn grill
    { .cmd_def.hash = HC(0x6bc1b5c2), .cmd_def.arg_def_cnt =  2, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0x61342fd0), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 1 }, // on
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0xab3a8a0a), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // off

    // help
    { .cmd_def.hash = HC(0x3871a3fa), .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },

    // end of list
    { .cmd_def.hash = HC(0x00000000), .cmd_def.arg_def_cnt = 0, .cmd_def.pos_cnt = 0, .cmd_def.pos_req = 0, .cmd_def.pos_multi = 0 },
};

const tcli_def_t tcli_def =
{
    .ca_def = &tcli_ca_def[0],
};
