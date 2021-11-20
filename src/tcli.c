#include <stdio.h>
#include <string.h>
#include "tcli.h"


static void tcli_tokenize(char *buf)
{
    enum {
        SKIPPING_SPACES,
        COPY,
    } state = SKIPPING_SPACES;
    char *f = buf;
    char *t = buf;
    char copy_until_char;

    while (*f)
    {
        switch (state)
        {
            case SKIPPING_SPACES:
            if (*f == ' ')
            {
                f++;
            }
            else
            {
                copy_until_char = (*f == '\'' || *f == '\"') ? *f++ : ' ';
                state = COPY;
            }
            break;

            case COPY:
            if ((*t = *f++) == copy_until_char)
            {
                *t = 0;
                state = SKIPPING_SPACES;
            }
            t++;
            break;
        }
    }
    *t = 0;

    puts("Tokenized command line:");
    t = buf;
    while (*t)
    {
        printf("\t%s\n", t);
        t += strlen(t) + 1;
    }
}

static tcli_cmd_def_t *find_cmd_def(const char **buf, tcli_def_t *tcli_def)
{
    int i;
    const tcli_cmd_def_t *cmd_def;
    const char *s;
    const char *cmd;

    puts("Searching through list of commands...\n");
    for (i = 0, cmd_def = tcli_def->cmd_def; i < CMD_ID_CNT; i++, cmd_def++)
    {
        printf("Trying cmd ID %d\n", i);
        s = *buf;
        cmd = cmd_def->str;
        while (*cmd)
        {
            printf("Matching '%s' with '%s': ", s, cmd);
            if (strcmp(s, cmd) == 0)
            {
                printf("yes\n");
            }
            else
            {
                printf("no\n");
                s = NULL;
                break;
            }
            s += strlen(s) + 1;
            cmd += strlen(cmd) + 1;
        }
        if (s)
        {
            *buf = s;
            return cmd_def;
        }
    }
    return NULL;
}


int tcli_parse(char *buf, const tcli_def_t *tcli_def)
{
    const tcli_cmd_def_t *cmd_def;

    tcli_tokenize(buf);
    cmd_def = find_cmd_def(&buf, tcli_def);
    printf("buf is now '%s'\n", buf);


    return 0;
}
