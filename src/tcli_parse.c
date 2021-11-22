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
            else if (*f == copy_until_char || (*f == '=' && copy_until_char == ' '))
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

static int find_cmd_def(const tcli_def_t *tcli_def, char **buf, const tcli_cmd_def_t **cmd_def)
{
    int i;
    int cmd_id;
    const tcli_cmd_def_t *cd;
    char *b;
    const char *s = tcli_def->cmd_string_tbl;

    cmd_id = TCLI_ERROR_COMMAND_NOT_FOUND;
    for (i = 0, cd = tcli_def->cmd_def; i < CMD_ID_CNT; i++, cd++)
    {
        DEBUG_PRINTF("Looking at cmd ID %d\n", cmd_id);
        b = *buf;
        if (cd->s1_idx)
        {
            DEBUG_PRINTF("Compare S1 '%s' to '%s'\n", &s[cd->s1_idx], b);
            if (!*b || strncmp(&s[cd->s1_idx], b, strlen(b)) != 0)
            {
                continue;
            }
            b += strlen(b) + 1;
        }
        if (cd->s2_idx)
        {
            DEBUG_PRINTF("Compare S1 '%s' to '%s'\n", &s[cd->s2_idx], b);
            if (!*b || strncmp(&s[cd->s2_idx], b, strlen(b)) != 0)
            {
                continue;
            }
            b += strlen(b) + 1;
        }
        DEBUG_PRINTF("Found it!\n");
        *buf = b;
        *cmd_def = cd;
        if (cmd_id > 0)
        {
            cmd_id = TCLI_ERROR_AMBIGUOUS_COMMAND;
            break;
        }
        cmd_id = i;
    }
    return cmd_id;
}

int tcli_parse(char *buf, const tcli_def_t *tcli_def, tcli_args_t *args)
{
    int i;
    int j;
    int cmd_id;
    int arg_def_cnt;
    const tcli_arg_def_t *arg_def;
    const tcli_cmd_def_t *cmd_def;
    const tcli_cmd_def_t *common_cmd_def = &tcli_def->cmd_def[0];
    uint32_t option_bit;
    uint32_t options_provided;
    uint32_t options_required;
    const char *s = &tcli_def->arg_string_tbl[0];

    tcli_tokenize(buf);
    DEBUG_PRINTF("Searching through list of commands...\n");
    cmd_id = find_cmd_def(tcli_def, &buf, &cmd_def);
    if (cmd_id < 0)
    {
        DEBUG_PRINTF("Command error\n");
    	return cmd_id;
    }
    DEBUG_PRINTF("Found command ID %d\n", cmd_id);
    options_provided = 0;
    options_required = 0;
    for (i = 0, arg_def = &tcli_def->arg_def[cmd_def->arg_def_idx]; i < cmd_def->arg_def_cnt; i++, arg_def++)
    {
        options_required |= (arg_def->required << arg_def->mutex_idx);
    }

    while (*buf)
    {
        DEBUG_PRINTF("Next arg is '%s'\n", buf);
        option_bit = 0;
        for (j = 0; j < 2; j++)
        {
            arg_def = (j == 0 ? &tcli_def->arg_def[common_cmd_def->arg_def_idx] : &tcli_def->arg_def[cmd_def->arg_def_idx]);
            arg_def_cnt = (j == 0 ? common_cmd_def->arg_def_cnt : cmd_def->arg_def_cnt);
            for (i = 0; i < arg_def_cnt && option_bit == 0; i++, arg_def++)
            {
                DEBUG_PRINTF("  Considering arg '%s'\n", &s[arg_def->long_idx]);
                switch (arg_def->type)
                {
                case ARG_TYPE_OPTION_BOOL:
                    if (strcmp(buf, &s[arg_def->long_idx]) == 0 || strcmp(buf, &s[arg_def->short_idx]) == 0)
                    {
                        DEBUG_PRINTF("That's it\n");
                        option_bit = (1 << arg_def->mutex_idx);
                        args->generic.bools |= (1 << arg_def->opt_idx);
                    }
                    break;
                case ARG_TYPE_OPTION_HAS_VALUE:
                    if (strcmp(buf, &s[arg_def->long_idx]) == 0 || strcmp(buf, &s[arg_def->short_idx]) == 0)
                    {
                        DEBUG_PRINTF("That's it\n");
                        option_bit = (1 << arg_def->mutex_idx);
                        buf += strlen(buf) + 1;
                        args->generic.args[arg_def->opt_idx] = buf;
                    }
                    break;
                case ARG_TYPE_POSITIONAL:
                case ARG_TYPE_POSITIONAL_MULTI:
                    if (!args->generic.args[arg_def->opt_idx])
                    {
                        args->generic.args[arg_def->opt_idx] = buf;
                        option_bit = (1 << arg_def->mutex_idx);
                        DEBUG_PRINTF("Positional arg %d filled in\n", arg_def->opt_idx);
                        if (arg_def->type == ARG_TYPE_POSITIONAL_MULTI)
                        {
                            DEBUG_PRINTF("Assigned multi arg\n");
                            while (*buf)
                            {
                                buf += strlen(buf) + 1;
                            }
                        }
                    }
                    break;
                default:
                    break;
                }
            }
        }
        if (!option_bit)
        {
            DEBUG_PRINTF("Unknown option\n");
            return TCLI_ERROR_UNKNOWN_OPTION;        	
        }
        if (option_bit & options_provided)
        {
            DEBUG_PRINTF("Option conflict\n");
            return TCLI_ERROR_OPTION_CONFLICT;
        }
        options_provided |= option_bit;
        if (*buf)
        {
            buf += strlen(buf) + 1;
        }
    }
    if ((options_required & options_provided) != options_required)
    {
        DEBUG_PRINTF("Required option not provided\n");
        return TCLI_ERROR_REQUIRED_OPTION_NOT_PROVIDED;
    }

    return cmd_id;
}

const char *next_arg(const char *arg)
{
    if (*arg)
    {
        arg += strlen(arg) + 1;
    }
    return *arg ? arg : (const char *) 0;
}
