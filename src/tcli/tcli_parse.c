#include <stdio.h>
#include <string.h>
#include "tcli.h"

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

static uint32_t hash;

static void tcli_hash_init(void)
{
    hash = 0x811c9dc5;
}

static void tcli_hash_add(const char *buf)
{
    while (*buf)
    {
        hash ^= *buf++;
        hash *= 0x01000193;
    }
}

static uint32_t tcli_hash_get_arg(void)
{
    return hash & ((1 << 22) - 1);
}

#if TCLI_USE_HASH_FOR_CMDS
static uint32_t tcli_hash_get_cmd(void)
{
    return hash & ((1 << 20) - 1);
}
#endif

static char *tcli_next(char *s)
{
    if (*s)
    {
        s += strlen(s) + 1;
    }
    return s;
}

static void tcli_tokenize(char *buf)
{
    char *f;
    char *t;
    char c;

    for (f = t = buf; *f;)
    {
        for (; *f == ' '; f++);
        if (*f == '\'' || *f == '\"')
        {
            for (c = *f++; *f && (*t = *f++) != c; t++);
        }
        else
        {
            for (; *f && (*t = *f++) != '=' && *t != ' '; t++);
        }
        *t++ = 0;
    }
    *t = 0;

#if DEBUG
    puts("Tokenized command line:");
    for (t = buf; *t; t = tcli_next(t))
    {
        printf("\t%s\n", t);
    }
#endif
}

#if TCLI_USE_HASH_FOR_CMDS

static int find_cmd_def(const tcli_def_t *tcli_def, char **buf_p, const tcli_cmd_def_t **cmd_def)
{
    int i;
    int cmd_id;
    const tcli_cmd_def_t *cd;
    char *buf = *buf_p;

    cmd_id = TCLI_ERROR_COMMAND_NOT_FOUND;
    tcli_hash_init();
    while (*buf && *buf != '-')
    {
        tcli_hash_add(buf);
        hash = tcli_hash_get_cmd();
        DEBUG_PRINTF("Adding '%s', hash is now 0x%08x\n", buf, hash);
        for (i = 1, cd = &tcli_def->ca_def->cmd_def; cd->hash != 0; i++, cd += cd->arg_def_cnt + 1)
        {
            DEBUG_PRINTF("Hash for cmd %d is 0x%08x\n", i, cd->hash);
            if (hash == cd->hash)
            {
                DEBUG_PRINTF("Found it!\n");
                *buf_p = tcli_next(buf);
                *cmd_def = cd;
                cmd_id = i;
            }
        }
        buf = tcli_next(buf);
    }
    DEBUG_PRINTF("Return cmd id %d\n", cmd_id);
    return cmd_id;
}

#else // TCLI_USE_HASH_FOR_CMDS

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
        DEBUG_PRINTF("Looking at cmd ID %d\n", i);
        b = buf;
        DEBUG_PRINTF("Compare S1 '%s' to '%s'\n", &s[cd->s1_idx], b);
        if (*b && strncmp(&s[cd->s1_idx], b, strlen(b)) == 0)
        {
            b = tcli_next(b);
            DEBUG_PRINTF("Compare S2 '%s' to '%s'\n", &s[cd->s2_idx], b);
            if (cd->s2_idx == 0 || (*b && strncmp(&s[cd->s2_idx], b, strlen(b)) == 0))
            {
                DEBUG_PRINTF("Found it!\n");
                if (cd->s2_idx)
                {
                    b = tcli_next(b);
                }
                if (cmd_id > 0)
                {
                    cmd_id = TCLI_ERROR_AMBIGUOUS_COMMAND;
                    break;
                }
                *buf_p = b;
                *cmd_def = cd;
                cmd_id = i;
            }
        }
    }
    return cmd_id;
}

#endif // TCLI_USE_HASH_FOR_CMDS

int tcli_parse(char *buf, const tcli_def_t *tcli_def, tcli_args_t *args)
{
    int i;
    int cmd_id;
    const tcli_arg_def_t *arg_def;
    const tcli_cmd_def_t *cmd_def;
    uint32_t option_bit;
    uint32_t options_provided = 0;
    uint32_t options_required = 0;
    int bool_idx;
    int val_idx;
    int mutex_idx;
    int pos_idx = 0;
    uint32_t hash;

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
        tcli_hash_init();
        tcli_hash_add(buf);
        hash = tcli_hash_get_arg();
        for (i = 0; i < cmd_def->arg_def_cnt && option_bit == 0; i++)
        {
            if (arg_def->has_val)
            {
                if (hash == arg_def->hash || (*buf == '-' && *(buf+1) == arg_def->short_char && *(buf+2) == 0))
                {
                    DEBUG_PRINTF("That's it\n");
                    option_bit = (1 << mutex_idx);
                    buf = tcli_next(buf);
                    args->generic.vals[val_idx] = buf;
                }
                val_idx++;
            }
            else
            {
                if (hash == arg_def->hash || (*buf == '-' && *(buf+1) == arg_def->short_char && *(buf+2) == 0))
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
                DEBUG_PRINTF("Arg conflict\n");
                return TCLI_ERROR_ARG_CONFLICT;
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
            args->generic.vals[val_idx + pos_idx++] = buf;
            if (pos_idx >= cmd_def->pos_cnt && cmd_def->pos_multi)
            {
                DEBUG_PRINTF("Assigned multi arg\n");
                for (; *buf; buf = tcli_next(buf));
            }
        }
        if (*buf)
        {
            buf = tcli_next(buf);
        }
    }
    if ((options_required & options_provided) != options_required || pos_idx < cmd_def->pos_req)
    {
        DEBUG_PRINTF("Required arg not provided\n");
        return TCLI_ERROR_REQUIRED_ARG_NOT_PROVIDED;
    }

    return cmd_id;
}

const char *tcli_next_arg(const char *arg)
{
    if (!arg || !*arg)
    {
        return NULL;
    }
    arg += strlen(arg) + 1;
    return *arg ? arg : NULL;
}
