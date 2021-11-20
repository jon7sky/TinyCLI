#include <stdio.h>
#include <string.h>
#include "tcli.h"


const tcli_cmd_def_t tcli_cmd_def[] =
{
    {
        .str = " \000"
    },
    {
        .str = "make\000burger\000"
    },
    {
        .str = "make\000fries\000"
    }
};
const tcli_def_t tcli_def =
{
    .cmd_def = &tcli_cmd_def
};

int main(int argc, char *argv[])
{
    char buf[1000];

    strcpy(buf, "make burger   --white --ketchup --mustard --name 'joe bob'");
    tcli_parse(buf, &tcli_def);

    return 0;
}
