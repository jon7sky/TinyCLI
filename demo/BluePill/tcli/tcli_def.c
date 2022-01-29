#include "tcli.h"

#define HA(x) ((x) & 0x003fffff)
#define HC(x) ((x) & 0x000fffff)

static const tcli_ca_def_t tcli_ca_def[] =
{
    // led
    { .cmd_def.hash = HC(0x406aeaca), .cmd_def.arg_def_cnt =  2, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0x61342fd0), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 1 }, // on
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0xab3a8a0a), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // off

    // bambi
    { .cmd_def.hash = HC(0x32ff609e), .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },

    // help
    { .cmd_def.hash = HC(0x3871a3fa), .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },

    // end of list
    { .cmd_def.hash = HC(0x00000000), .cmd_def.arg_def_cnt = 0, .cmd_def.pos_cnt = 0, .cmd_def.pos_req = 0, .cmd_def.pos_multi = 0 },
};

const tcli_def_t tcli_def =
{
    .ca_def = &tcli_ca_def[0],
};
