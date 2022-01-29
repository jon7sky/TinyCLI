#include "tcli.h"

#define PRINTF_ARG(...)
// #include <stdio.h>
// #define PRINTF_ARG(...) printf(__VA_ARGS__)

static tcli_args_t args;

int tcli_cmd_handle(char *buf)
{
    int rc;

    memset(&args, 0, sizeof(args));
    if ((rc = tcli_parse(buf, &tcli_def, &args)) < 0) return rc;

    switch (rc)
    {
    case CMD_ID_led:                          return tcli_cmd_handle_led(&args.led);
    case CMD_ID_button_wait:                  return tcli_cmd_handle_button_wait(&args.button_wait);
    case CMD_ID_help:                         return tcli_cmd_handle_help(&args.help);
    default:                                  return TCLI_INTERNAL_ERROR;
    }
}

__attribute__((weak)) int tcli_cmd_handle_led(tcli_args_led_t *args)
{
    PRINTF_ARG("on:                  %d\n", args->on);
    PRINTF_ARG("off:                 %d\n", args->off);
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_button_wait(tcli_args_button_wait_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_help(tcli_args_help_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

