#include "tcli.h"

const char *tcli_error(int error)
{
    switch (error)
    {
    case TCLI_OK:                                   return("OK");
    case TCLI_ERROR_COMMAND_NOT_FOUND:              return("Unrecognized command");
    case TCLI_ERROR_ARG_CONFLICT:                   return("Conflicting arguments");
    case TCLI_ERROR_REQUIRED_ARG_NOT_PROVIDED:      return("Required argument not provided");
    case TCLI_ERROR_COMMAND_NOT_SUPPORTED:          return("Command not supported");
    case TCLI_ERROR_UNKNOWN_OPTION:                 return("Unrecognized option");
    case TCLI_ERROR_AMBIGUOUS_COMMAND:              return("Ambiguous command");
    case TCLI_INTERNAL_ERROR:                       return("Internal error");
    case TCLI_ERROR_TOO_MANY_ARGS:                  return("Too many arguments");
    case TCLI_ERROR_MALFORMED_ARG:                  return("Malformed argument");
    default:                                        return("Unknown error code");
    }
}
