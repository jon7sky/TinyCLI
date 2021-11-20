#include <stdio.h>
#include <string.h>
#include "tcli.h"

const tcli_arg_def_t arg_def_make_burger[] =
{
	{ .s = "--white",		.type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 0,   .mutex_idx = 0, .required = 1 },
	{ .s = "--wheat",		.type = ARG_TYPE_OPTION_BOOL,		.opt_idx = 1,   .mutex_idx = 0, .required = 1 },
	{ .s = "--ketchup",		.type = ARG_TYPE_OPTION_BOOL,		.opt_idx = 2,   .mutex_idx = 1, .required = 0 },
	{ .s = "--mustard",		.type = ARG_TYPE_OPTION_BOOL,		.opt_idx = 3,   .mutex_idx = 2, .required = 0 },
	{ .s = "--name=",		.type = ARG_TYPE_OPTION_HAS_VALUE,  .opt_idx = 0,   .mutex_idx = 3, .required = 0 }
};

const tcli_arg_def_t arg_def_make_fries[] =
{
	{ .s = "--extra-salt", 	.type = ARG_TYPE_OPTION_BOOL,       .opt_idx = 0,   .mutex_idx = 0, .required = 0 },
};

const tcli_cmd_def_t tcli_cmd_def[] =
{
    { .s = " \000",                 .slen = 2,  .arg_def = NULL,                        .arg_def_cnt = 0 },
    { .s = "make\000burger\000",    .slen = 12, .arg_def = &arg_def_make_burger[0],     .arg_def_cnt = 5 },
    { .s = "make\000fries\000",     .slen = 11, .arg_def = &arg_def_make_fries[0],      .arg_def_cnt = 1 }
};

const tcli_def_t tcli_def =
{
    .cmd_def = &tcli_cmd_def[0]
};

tcli_args_t args;

int main(int argc, char *argv[])
{
    char buf[1000];

    strcpy(buf, "make burger   --white --ketchup --mustard --name=BillyJoeBob");
    tcli_parse(buf, &tcli_def, &args);

    return 0;
}
