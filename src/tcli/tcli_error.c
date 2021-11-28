#include "tcli.h"

const char *tcli_error(int error)
{
    switch (error)
    {
    case TCLI_OK:                                   return("OK");
    case TCLI_ERROR_COMMAND_NOT_FOUND:              return("Unrecognized command");
    case TCLI_ERROR_OPTION_CONFLICT:                return("Conflicting options");
    case TCLI_ERROR_REQUIRED_OPTION_NOT_PROVIDED:   return("Required option not provided");
    case TCLI_ERROR_COMMAND_NOT_SUPPORTED:          return("Command not supported");
    case TCLI_ERROR_UNKNOWN_OPTION:                 return("Unrecognized option");
    case TCLI_ERROR_AMBIGUOUS_COMMAND:              return("Ambiguous command");
    case TCLI_INTERNAL_ERROR:                       return("Internal error");
    case TCLI_ERROR_TOO_MANY_ARGS:                  return("Too many arguments");
    default:                                        return("Unknown error code");
    }
}
