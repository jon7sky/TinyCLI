#include <stdio.h>
#include <string.h>
#include "tcli.h"

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif


static void tcli_tokenize(char *buf)
{
    char *f;
    char *t;
    char copy_until_char;

    for (f = t = buf; *f;)
    {
        while (*f == ' ')
        {
            f++;
        }
        copy_until_char = ' ';
        while (*f)
        {
            if ((*f == '\'' || *f == '\"') && copy_until_char == ' ')
            {
                copy_until_char = *f;
            }
            else if (*f == copy_until_char)
            {
                *t++ = 0;
                f++;
                break;
            }
            else
            {
                *t++ = *f;
            }
            f++;
        }
    }
    *t++ = 0;
    *t = 0;

#if DEBUG
    puts("Tokenized command line:");
    t = buf;
    while (*t)
    {
        printf("\t%s\n", t);
        t += strlen(t) + 1;
    }
#endif
}

int tcli_parse(char *buf, const tcli_cmd_def_t *cmd_def, tcli_args_t *args)
{
    int i;
    int cmd_id;
    const tcli_arg_def_t *arg_def;
    uint32_t option_bit;
    uint32_t options_provided;
    uint32_t options_required;

    tcli_tokenize(buf);
    DEBUG_PRINTF("Searching through list of commands...\n");
    for (cmd_id = 0; cmd_id < CMD_ID_CNT; cmd_id++, cmd_def++)
    {
    	if (memcmp(buf, cmd_def->s, cmd_def->slen) == 0)
    	{
    		break;
    	}
    }
    if (cmd_id == CMD_ID_CNT)
    {
        DEBUG_PRINTF("Command not found\n");
    	return TCLI_ERROR_COMMAND_NOT_FOUND;
    }
    DEBUG_PRINTF("Found command ID %d\n", cmd_id);
    buf += cmd_def->slen;
    options_provided = 0;
    options_required = 0;
    for (i = 0, arg_def = cmd_def->arg_def; i < cmd_def->arg_def_cnt; i++, arg_def++)
    {
        options_required |= (arg_def->required << arg_def->mutex_idx);
    }

    while (*buf)
    {
        DEBUG_PRINTF("Next arg is '%s'\n", buf);
        option_bit = 0;
        for (i = 0, arg_def = cmd_def->arg_def; i < cmd_def->arg_def_cnt && option_bit == 0; i++, arg_def++)
        {
            DEBUG_PRINTF("  Considering arg '%s'\n", arg_def->s);
            switch (arg_def->type)
            {
            case ARG_TYPE_OPTION_BOOL:
                if (strcmp(buf, arg_def->s) == 0)
                {
                    DEBUG_PRINTF("That's it\n");
                    option_bit = (1 << arg_def->mutex_idx);
                    args->generic.bools |= (1 << arg_def->opt_idx);
                }
                break;
            case ARG_TYPE_OPTION_HAS_VALUE:
                if (strncmp(buf, arg_def->s, strlen(arg_def->s)) == 0)
                {
                    DEBUG_PRINTF("That's it\n");
                    option_bit = (1 << arg_def->mutex_idx);
                    args->generic.args[arg_def->opt_idx] = buf + strlen(arg_def->s);
                }
                break;
            default:
                break;
            }
        }
        if (option_bit & options_provided)
        {
            DEBUG_PRINTF("Option conflict\n");
            return TCLI_ERROR_OPTION_CONFLICT;
        }
        options_provided |= option_bit;
        buf += strlen(buf) + 1;
    }
    if ((options_required & options_provided) != options_required)
    {
        DEBUG_PRINTF("Required option not provided\n");
        return TCLI_ERROR_REQUIRED_OPTION_NOT_PROVIDED;
    }

    return cmd_id;
}
