#include <string.h>
#include "tcli.h"

static tcli_args_t args;

const char *tcli_error(int error)
{
    switch (error)
    {

    case TCLI_OK:                                   return("OK");
    case TCLI_ERROR_COMMAND_NOT_FOUND:              return("Unrecognized command");
    case TCLI_ERROR_OPTION_CONFLICT:                return("Conflicting options");
    case TCLI_ERROR_REQUIRED_OPTION_NOT_PROVIDED:   return("Required option not provided");
    case TCLI_ERROR_COMMAND_NOT_SUPPORTED:          return("Command not supported");
    case TCLI_ERROR_UNKNOWN_OPTION:                 return("Unrecognized option");
    case TCLI_ERROR_AMBIGUOUS_COMMAND:              return("Ambiguous command");
    case TCLI_INTERNAL_ERROR:                       return("Internal error");
    default:                                        return("Unknown error code");
    }
}

int tcli_cmd_handle(char *buf)
{
    int rc;

    memset(&args, 0, sizeof(args));
    rc = tcli_parse(buf, &tcli_def, &args);
    if (rc < 0)
    {
        return rc;
    }

    switch (rc)
    {
    case CMD_ID_make_burger:    return tcli_cmd_handle_make_burger(&args.make_burger);
    case CMD_ID_make_fries:     return tcli_cmd_handle_make_fries(&args.make_fries);
    case CMD_ID_deliver_to:     return tcli_cmd_handle_deliver_to(&args.deliver_to);
    case CMD_ID_eat:            return tcli_cmd_handle_eat(&args.eat);
    case CMD_ID_turn_grill:     return tcli_cmd_handle_turn_grill(&args.turn_grill);
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

__attribute__((weak)) int tcli_cmd_handle_deliver_to(tcli_args_deliver_to_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_eat(tcli_args_eat_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_turn_grill(tcli_args_turn_grill_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}
