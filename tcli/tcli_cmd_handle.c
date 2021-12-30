// WARNING: This file has been automatically generated.
// Do not edit, or your changes will be lost the next time code is generated.

#include "tcli.h"

#define TCLI_CMD_HANDLE_DEBUG 0

#if TCLI_CMD_HANDLE_DEBUG
#include <stdio.h>
#endif

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
#if TCLI_CMD_HANDLE_DEBUG
    printf("white:               %d\n", args->white);
    printf("wheat:               %d\n", args->wheat);
    printf("ketchup:             %d\n", args->ketchup);
    printf("mustard:             %d\n", args->mustard);
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

__attribute__((weak)) int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args)
{
#if TCLI_CMD_HANDLE_DEBUG
    printf("light_salt:          %d\n", args->light_salt);
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

__attribute__((weak)) int tcli_cmd_handle_make_biscuits(tcli_args_make_biscuits_t *args)
{
#if TCLI_CMD_HANDLE_DEBUG
    printf("gravy:               %d\n", args->gravy);
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

__attribute__((weak)) int tcli_cmd_handle_make_grits(tcli_args_make_grits_t *args)
{
#if TCLI_CMD_HANDLE_DEBUG
    printf("butter:              %d\n", args->butter);
    printf("sugar:               %d\n", args->sugar);
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

__attribute__((weak)) int tcli_cmd_handle_deliver_to(tcli_args_deliver_to_t *args)
{
#if TCLI_CMD_HANDLE_DEBUG
    printf("addr:                '%s'\n", args->addr ? args->addr : "NULL");
    printf("name:                '%s'\n", args->name ? args->name : "NULL");
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

__attribute__((weak)) int tcli_cmd_handle_eat(tcli_args_eat_t *args)
{
#if TCLI_CMD_HANDLE_DEBUG
    printf("food:                ");
    {
        const char *p;
        for (p = args->food; p != NULL; p = tcli_next_arg(p))
        {
            printf("'%s' ", p);
        }
        printf("\n");
    }
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

__attribute__((weak)) int tcli_cmd_handle_turn_grill(tcli_args_turn_grill_t *args)
{
#if TCLI_CMD_HANDLE_DEBUG
    printf("on:                  %d\n", args->on);
    printf("off:                 %d\n", args->off);
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

__attribute__((weak)) int tcli_cmd_handle_help(tcli_args_help_t *args)
{
#if TCLI_CMD_HANDLE_DEBUG
    return TCLI_OK;
#else
    return TCLI_ERROR_COMMAND_NOT_SUPPORTED;
#endif
}

