# TinyCLI
## A small C-language command-line interpreter for embedded systems

### Command Line Definition File

TinyCLI got its inspiraiton for its command line definition file from DocOpt, which allows you to
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
    
Let's break this down

### Commands

In the above example, the commands are "make burger", "make fries", etc. You type those exact words, in that exact order.
