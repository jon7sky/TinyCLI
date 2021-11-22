#include <stdio.h>
#include <string.h>
#include "tcli.h"

tcli_args_t args;

int main(int argc, char *argv[])
{
    char buf[1000];
    const char **cmd;
    int rc;

    const char *test_cmds[] =
    {
        "make burger   --white --ketchup --mustard --name='Alice Jones'",
        "m b --wheat --ketchup --mustard --name='Bob Smith'",
        "make fries",
        "deliver to Alice",
        "deliver to Bob '123 Main St'",
        "deliver to Charlie '666 Elm St' huh?",
        "deliver to",
        "eat burger fries",
        NULL
    };

    for (cmd = &test_cmds[0]; *cmd; cmd++)
    {
        printf("Command: %s\n", *cmd);
        strcpy(buf, *cmd);
        rc = tcli_cmd_handle(buf);
        printf("Command returned %d\n\n", rc);
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
    return TCLI_OK;
}

int tcli_cmd_handle_deliver_to(tcli_args_deliver_to_t *args)
{
    printf("Delivering order to %s", args->person);
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

    for (food = args->food; food; food = next_arg(food))
    {
        printf("Eating: %s\n", food);
    }
    return TCLI_OK;
}
