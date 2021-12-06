#include "tcli.h"

#define HA(x) ((x) & 0x003fffff)
#define HC(x) ((x) & 0x000fffff)

static const tcli_ca_def_t tcli_ca_def[] =
{
    // led
    { .cmd_def.hash = HC(0x406aeaca), .cmd_def.arg_def_cnt =  6, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0x40f480dc), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 1 }, // red
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0x011decbc), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // green
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0x82fbf5cd), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // blue
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0x13254bc4), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // all
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0x61342fd0), .arg_def.has_val = 0, .arg_def.mutex = 1, .arg_def.required = 1 }, // on
    { .arg_def.short_char = ' ', .arg_def.hash = HA(0xab3a8a0a), .arg_def.has_val = 0, .arg_def.mutex = 0, .arg_def.required = 1 }, // off

    // button wait
    { .cmd_def.hash = HC(0xbfcf271c), .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },

    // help
    { .cmd_def.hash = HC(0x3871a3fa), .cmd_def.arg_def_cnt =  0, .cmd_def.pos_cnt =  0, .cmd_def.pos_req =  0, .cmd_def.pos_multi = 0 },

    // end of list
    { .cmd_def.hash = HC(0x00000000), .cmd_def.arg_def_cnt = 0, .cmd_def.pos_cnt = 0, .cmd_def.pos_req = 0, .cmd_def.pos_multi = 0 },
};

const tcli_def_t tcli_def =
{
    .ca_def = &tcli_ca_def[0],
};
