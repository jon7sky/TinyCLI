#include <stdint.h>
#include "tty.h"

int _read(int file, char *ptr, int len)
{
    //return tty_rx((uint8_t *)ptr, len);
    return EOF;
}

int _write(int file, char *ptr, int len) {
    tty_tx((uint8_t *) ptr, len);
    return len;
}
