#include <stdio.h>
#include <stdarg.h>

#include "ch.h"
#include "chprintf.h"
#include "memstreams.h"

#include <solarnode_debug.h>

void dbglog(const char *fmt, ...) {
#ifdef _DEBUG
    va_list ap;
    MemoryStream ms;
    BaseSequentialStream *chp;

    char str[64];

    msObjectInit(&ms, (uint8_t *)&str, 64 - 1, 0);

    /* Performing the print operation using the common code.*/
    chp = (BaseSequentialStream *)(void *)&ms;
    va_start(ap, fmt);
    chvprintf(chp, fmt, ap);
    va_end(ap);

    /* Terminate with a zero, unless size==0.*/
    if (ms.eos < 64)
        str[ms.eos] = 0;
    puts(str);
#endif
}

