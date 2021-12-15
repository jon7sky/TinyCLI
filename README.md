# TinyCLI
## A Small C-language Command-Line Interpreter For Embedded Systems

TinyCLI is a command-line interpreter library that was written with embedded projects in mind.
The parser uses the C language. You pass it a character array, and it parses the command and
calls one of several command handler functions that you provide. Each command handler is passed
one parameter, which is a pointer to a structure that contains all the arguments.

### Command Line Definition File

TinyCLI got its inspiraiton for its command line definition file from `docopt`, which allows you to
"create beautiful command-line interfaces with Python." The commands are defined in a very
familiar-looking usage pattern like this:

```
make burger [options] (-w,--white | -h,--wheat)
    -k, --ketchup       Add ketchup 
    -m, --mustard       Add Mustard
make fries [-l,--light-salt]
deliver to <name> [--addr=<address>]
eat <food>...
turn grill (on | off)
```

A C code generator written in Python will parse this definition file and generate the C and H files necessary for your parser.

### Commands

In the above example, the commands are `make burger`, `make fries`, etc. Those are the exact words that need to appear on your command line, in that order.

### Short vs Long Option Names

Short option names start with a single `-`, while long option names start with a double `--`.
If both a short and a long name are desired for an option, they are separated by a single comma. For example:
```
make burger [-k,--ketchup]
```
Either `make burger -k` or `make burger --ketchup` is vaid.

### Options That Require Values vs Boolean Flags

Options that require a value must be specified with a `=`, while options that are just a true/false flag don't use the `=`. For example:
```
pour drink --type=<type> --no-ice
```
The option `type` requires a value, but `no-ice` does not, like `pour drink --type=beer --no-ice`.

### Required vs Optional Arguments

Required arguments are surrounded with `()`, and optional arguments are surrounded with `[]`. 
If an argument is specified with nether, it considered to be optional. For example:
```
pour drink (--type=<type>) [--no-ice]
```
Both `pour drink --type=beer --no-ice` or `pour drink --type=beer` are valid because `type` is specified, 
but `pour drink` or `pour drink --no-ice` are invalid because `type` is not specified.

### Mutually Exclusive Options

A `|` between two or more options means that only one may be provided. For example:
```
make toast (--white | --wheat | --rye)
```
Both `make toast --white` and `make toast --rye` are valid, but `make toast --white --rye` is invalid because only one of those options can be specified.
Also, `make toast` is invalid because the `()` means that one of the options in the list is required.

You don't need to use `-` or `--` for the options. For example:
```
turn light (on | off)
```
means that either `turn light on` or `turn light off` is valid.

### Positional Arguments

Positional arguments are surrounded by `<>`. Adding three dots after it means that it can stand for one or more things. For example:
```
eat <food>
```
Both `eat burger` and `eat fries` are valid but not `eat burger fries`. However, with:
```
eat <food>...
```
`eat burger fries cookie` is valid. But you must provide at least one argument. If you would like to specify zero or more arguments, use:
```
eat [<food>...]
```

### Multi-Word Arguments

Arguments that are more than one word can be surrounded by single `''` or double `""`. For example, for the `pour drink` command specified above, 
`pour drink --type='root beer'` and `pour drink --type="ginger ale"` are valid. An empty string for an argument can be given
by just using `''` or `""`, such as `pour drink --type=''`.

### Multi-Line Command Definition

Commands can be defined on multiple lines for easier reading, like this:
```
make burger [options] (-w,--white | -h,--wheat)
    -k, --ketchup       Add ketchup 
    -m, --mustard       Add Mustard
```
Here are the rules:
* A line containing the beginning of the command definition must start with an alpha character.
* A line containing additional options must start with a `-`.
* The word `[options]` doesn't actually do anything and is ignored.
* There can be zero or one space after the `,` when separating the short and long names. Using two or more spaces will confuse the code generator.
* The text after the options is just a comment and is ignored.

## Processing The Command Line Definition File

In the example code, the command line definition file is called `tcli_def.txt`.
Edit this file and define all your commands. When you are done, run the Python script `tcli_gen.py`
and it will parse the definiton file and generate the required C and H files.

## Argument Structure

A structure used for holding all the arguments is generated for each command.
Boolean flags are stored in a single bit, while arguments that require values are
stored as a character pointer. If a argument is not provided on the command line, its field in the structure will be zero.
For example, a command specified like this:
```
pour drink (--type=<type>) [--no-ice]
```
will store its arguments in a structure defined like this:
```
typedef struct
{
    uint32_t no_ice:1
    uint32_t _pad_:31;
    char *type;
} tcli_args_pour_drink_t;
```

## The Command Handler

A command entered by the user must be stored in a `char[]` buffer. This bufer is passed to `tcli_cmd_handle()` in `tcli_cmd_handle.c`.
This function will parse the command line, fill in an argument structure, and call a function named `tcli_command_handle_<name_of_your_command>()`.
For example, the `pour drink` command will fill in a `tcli_args_pour_drink_t` argument structure and pass it to a `tcli_command_handle_pour_drink()` function.
Default implementations for all valid commands are defined in `tcli_command_handle.c` with the `__weak__` attribute so they can be easily overridden.
It is expected that the user defines the real implementation of each of those functions in another file.

## Implementation Shortcuts

Since this library was written with embedded applications in mind, several shortcuts were taken to keep the code small.
* Commands are hashed. This means that the hash for the command `e` and the hash for the command `eat some food`
take the same amount of stoage, which is good, because that means that this library does not limit how many words are in your commands. 
But, this also means that it's possible to enter some random characters that result in the same hash as one of your commands.
* A maximum of 32 boolean options can be specified for each command.
* The command buffer is destroyed when parsed. The tokenizer will write null terminators after each word on the command line.
* While `=` must be used when defining commands, the parser will not require it. For example `pour drink --type=beer` and `pour drink --type beer` both work.
* It is not possible to combine multiple short options, for example, you must use `make burger -m -k` and not `make burger -mk`.
