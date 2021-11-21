#include "tcli.h"

int tcli_cmd_handle(char *buf, const tcli_cmd_def_t *tcli_def, tcli_args_t *args)
{
    int rc;

    rc = tcli_parse(buf, tcli_def, args);
    if (rc < 0)
    {
        return rc;
    }

    return rc;
}

__attribute__((weak)) int tcli_cmd_handle_make_burger(tcli_args_make_burger_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}
