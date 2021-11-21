#include "tcli.h"

static tcli_args_t args;

int tcli_cmd_handle(char *buf)
{
    int rc;

    rc = tcli_parse(buf, &tcli_cmd_def[0], &args);
    if (rc < 0)
    {
        return rc;
    }

    switch (rc)
    {
    case CMD_ID_make_burger:    return tcli_cmd_handle_make_burger(&args.make_burger);
    case CMD_ID_make_fries:     return tcli_cmd_handle_make_fries(&args.make_fries);
    default:                    return TCLI_INTERNAL_ERROR;
    }
}

__attribute__((weak)) int tcli_cmd_handle_make_burger(tcli_args_make_burger_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}
