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
    case CMD_ID_make_burger:                  return tcli_cmd_handle_make_burger(&args.make_burger);
    case CMD_ID_make_fries:                   return tcli_cmd_handle_make_fries(&args.make_fries);
    case CMD_ID_make_biscuits:                return tcli_cmd_handle_make_biscuits(&args.make_biscuits);
    case CMD_ID_make_grits:                   return tcli_cmd_handle_make_grits(&args.make_grits);
    case CMD_ID_deliver_to:                   return tcli_cmd_handle_deliver_to(&args.deliver_to);
    case CMD_ID_eat:                          return tcli_cmd_handle_eat(&args.eat);
    case CMD_ID_turn_grill:                   return tcli_cmd_handle_turn_grill(&args.turn_grill);
    case CMD_ID_help:                         return tcli_cmd_handle_help(&args.help);
    default:                                  return TCLI_INTERNAL_ERROR;
    }
}

__attribute__((weak)) int tcli_cmd_handle_make_burger(tcli_args_make_burger_t *args)
{
    PRINTF_ARG("white:               %d\n", args->white);
    PRINTF_ARG("wheat:               %d\n", args->wheat);
    PRINTF_ARG("ketchup:             %d\n", args->ketchup);
    PRINTF_ARG("mustard:             %d\n", args->mustard);
    PRINTF_ARG("to_go:               %d\n", args->to_go);
    PRINTF_ARG("name:                '%s'\n", args->name ? args->name : "NULL");
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args)
{
    PRINTF_ARG("light_salt:          %d\n", args->light_salt);
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_make_biscuits(tcli_args_make_biscuits_t *args)
{
    PRINTF_ARG("gravy:               %d\n", args->gravy);
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_make_grits(tcli_args_make_grits_t *args)
{
    PRINTF_ARG("butter:              %d\n", args->butter);
    PRINTF_ARG("sugar:               %d\n", args->sugar);
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_deliver_to(tcli_args_deliver_to_t *args)
{
    PRINTF_ARG("name:                '%s'\n", args->name ? args->name : "NULL");
    PRINTF_ARG("address:             '%s'\n", args->address ? args->address : "NULL");
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_eat(tcli_args_eat_t *args)
{
    PRINTF_ARG("food:                '%s'\n", args->food ? args->food : "NULL");
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_turn_grill(tcli_args_turn_grill_t *args)
{
    PRINTF_ARG("on:                  %d\n", args->on);
    PRINTF_ARG("off:                 %d\n", args->off);
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

__attribute__((weak)) int tcli_cmd_handle_help(tcli_args_help_t *args)
{
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
}

