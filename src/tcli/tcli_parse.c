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

static int find_cmd_def(const tcli_def_t *tcli_def, char **buf_p, const tcli_cmd_def_t **cmd_def)
{
    int i;
    int cmd_id;
    const tcli_cmd_def_t *cd;
    char *b;
    char *buf = *buf_p;
    const char *s = tcli_def->cmd_string_tbl;

    cmd_id = TCLI_ERROR_COMMAND_NOT_FOUND;
    for (i = 1, cd = &tcli_def->ca_def->cmd_def; cd->s1_idx != 0; i++, cd += cd->arg_def_cnt + 1)
    {
        if (cd->s1_idx == 0)
        {
            continue;
        }
        DEBUG_PRINTF("Looking at cmd ID %d\n", i);
        b = buf;
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
            DEBUG_PRINTF("Compare S2 '%s' to '%s'\n", &s[cd->s2_idx], b);
            if (!*b || strncmp(&s[cd->s2_idx], b, strlen(b)) != 0)
            {
                continue;
            }
            b += strlen(b) + 1;
        }
        DEBUG_PRINTF("Found it!\n");
        *buf_p = b;
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
    int cmd_id;
    const tcli_arg_def_t *arg_def;
    const tcli_cmd_def_t *cmd_def;
    uint32_t option_bit;
    uint32_t options_provided = 0;
    uint32_t options_required = 0;
    const char *s = &tcli_def->arg_string_tbl[0];
    int bool_idx;
    int val_idx;
    int mutex_idx;
    int pos_idx = 0;

    tcli_tokenize(buf);
    DEBUG_PRINTF("Searching through list of commands...\n");
    cmd_id = find_cmd_def(tcli_def, &buf, &cmd_def);
    if (cmd_id < 0)
    {
        DEBUG_PRINTF("Command error\n");
    	return cmd_id;
    }
    DEBUG_PRINTF("Found command ID %d\n", cmd_id);

    for (i = 0, mutex_idx = 0, arg_def = (tcli_arg_def_t *)cmd_def + 1; i < cmd_def->arg_def_cnt; i++)
    {
        options_required |= (arg_def->required << mutex_idx);
        arg_def++;
        mutex_idx += arg_def->mutex;
    }
    DEBUG_PRINTF("Options required: 0x%08x\n", options_required);

    while (*buf)
    {
        DEBUG_PRINTF("Next arg is '%s'\n", buf);
        option_bit = 0;
        val_idx = bool_idx = 0;
        mutex_idx = 0;
        arg_def = (tcli_arg_def_t *)cmd_def + 1;
        for (i = 0; i < cmd_def->arg_def_cnt && option_bit == 0; i++)
        {
            DEBUG_PRINTF("  Considering arg '%s', mutex_idx = %d\n", &s[arg_def->long_idx], mutex_idx);
            if (arg_def->has_val)
            {
                if (strcmp(buf, &s[arg_def->long_idx]) == 0 || (*buf == '-' && *(buf+1) == arg_def->short_char && *(buf+2) == 0))
                {
                    DEBUG_PRINTF("That's it\n");
                    option_bit = (1 << mutex_idx);
                    buf += strlen(buf) + 1;
                    args->generic.args[val_idx] = buf;
                }
                val_idx++;
            }
            else
            {
                if (strcmp(buf, &s[arg_def->long_idx]) == 0 || (*buf == '-' && *(buf+1) == arg_def->short_char && *(buf+2) == 0))
                {
                    DEBUG_PRINTF("That's it\n");
                    option_bit = (1 << mutex_idx);
                    args->generic.bools |= (1 << bool_idx);
                }
                bool_idx++;
            }
            arg_def++;
            mutex_idx += arg_def->mutex;
        }

        if (option_bit)
        {
            if (option_bit & options_provided)
            {
                DEBUG_PRINTF("Option conflict\n");
                return TCLI_ERROR_OPTION_CONFLICT;
            }
            options_provided |= option_bit;
        }
        else
        {
            if (*buf == '-')
            {
                return TCLI_ERROR_UNKNOWN_OPTION;
            }
            if (pos_idx >= cmd_def->pos_cnt)
            {
                return TCLI_ERROR_TOO_MANY_ARGS;
            }
            DEBUG_PRINTF("Positional arg %d filled in\n", val_idx + pos_idx);
            args->generic.args[val_idx + pos_idx++] = buf;
            if (pos_idx >= cmd_def->pos_cnt && cmd_def->pos_multi)
            {
                DEBUG_PRINTF("Assigned multi arg\n");
                for (; *buf; buf += strlen(buf) + 1);
            }
        }
        if (*buf)
        {
            buf += strlen(buf) + 1;
        }
    }
    if ((options_required & options_provided) != options_required || pos_idx < cmd_def->pos_req)
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
