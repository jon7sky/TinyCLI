#include <stdio.h>
#include <string.h>
#include "tcli.h"


#if 0
const tcli_def_t tcli_def =
{
    .cmd_def = &tcli_cmd_def[0]
};
#endif

tcli_args_t args;

int main(int argc, char *argv[])
{
    char buf[1000];

    strcpy(buf, "make burger   --white --ketchup --mustard --name='Billy Joe Bob'");
    tcli_parse(buf, &tcli_cmd_def, &args);

    strcpy(buf, "make burger   --white --wheat --ketchup --mustard --name=BillyJoeBob");
    tcli_parse(buf, &tcli_cmd_def, &args);

    strcpy(buf, "make burger   --ketchup --mustard --name=BillyJoeBob");
    tcli_parse(buf, &tcli_cmd_def, &args);

    strcpy(buf, "make fries --extra-salt --extra-salt");
    tcli_parse(buf, &tcli_cmd_def, &args);

    return 0;
}
