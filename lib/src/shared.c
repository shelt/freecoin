#include "shared.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void init()
{
    io_init();
    util_init();
    
}

void fatal(const char *string, ...)
{
    va_list args;
    va_start(args, string);

    printf("FATAL: ");
    vfprintf (stdout, string, args);
    printf("\n");
    
    exit(1);
};

void warn(const char *string, ...)
{
    va_list args;
    va_start(args, string);

    printf("WARN: ");
    vfprintf (stdout, string, args);
    printf("\n");
    
    exit(1);
};