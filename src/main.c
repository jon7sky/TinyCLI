#include <stdio.h>
#include <string.h>
#include "tcli.h"

int main(int argc, char *argv[])
{
    char buf[1000];
    const char **cmd;
    int rc;

    const char *test_cmds[] =
    {
        "make   burger   --wheat   --ketchup   --mustard   --name='Alice Jones'",
        "m bu -w -k -n 'Bob Smith' -g",
        "m b -w -k -n 'Bob Smith' -g",
        "make fries",
        "make fries --light-salt",
        "make fries --extra-salt",
        "deliver to Alice",
        "deliver to Bob '123 Main St'",
        "deliver to Charlie '666 Elm St' huh?",
        "deliver to",
        "eat burger fries shake",
        "turn grill on",
        "turn grill off",
        "turn grill on off",
        "turn grill",
        "hey you",
        "help",
        NULL
    };

    for (cmd = &test_cmds[0]; *cmd; cmd++)
    {
        printf("Command: %s\n", *cmd);
        strcpy(buf, *cmd);
        rc = tcli_cmd_handle(buf);
        printf("Command returned %d : %s\n\n", rc, tcli_error(rc));
    }

    return 0;
}

int tcli_cmd_handle_make_burger(tcli_args_make_burger_t *args)
{
    printf("Making a burger for %s\n", args->name);
    printf("Bun is %s\n", args->wheat ? "wheat" : "white");
    if (args->ketchup)
    {
        printf("Add ketchup\n");
    }
    if (args->mustard)
    {
        printf("Add mustard\n");
    }
    if (args->to_go)
    {
        printf("Order is to-go\n");
    }
    return TCLI_OK;
}

int tcli_cmd_handle_make_fries(tcli_args_make_fries_t *args)
{
    printf("Making fries");
    if (args->light_salt)
    {
        printf(" with light salt");
    }
    printf("\n");
    return TCLI_OK;
}

int tcli_cmd_handle_deliver_to(tcli_args_deliver_to_t *args)
{
    printf("Delivering order to %s", args->name);
    if (args->address)
    {
        printf(" at %s", args->address);
    }
    printf("\n");
    return TCLI_OK;
}

int tcli_cmd_handle_eat(tcli_args_eat_t *args)
{
    const char *food;

    for (food = args->food; food; food = tcli_next_arg(food))
    {
        printf("Eating: %s\n", food);
    }
    return TCLI_OK;
}

int tcli_cmd_handle_turn_grill(tcli_args_turn_grill_t *args)
{
    printf("Turning grill %s\n", args->on ? "on" : "off");
    return TCLI_OK;
}

int tcli_cmd_handle_help(tcli_args_help_t *args)
{
    puts(TCLI_USAGE_TEXT);
    return TCLI_OK;
}
