#include "fs.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

char *m_strconcat(int count, ...)
{
    va_list ap;
    
    char *buffer = malloc(MAX_STR_LEN);
    buffer[0] = '\0';
    
    va_start(ap, count);
    for (int i=0; i<count; i++)
        strcat(buffer, va_arg(ap, char *));
    va_end(ap);
    
    int len = strlen(buffer) + 1;
    char *retval = malloc(len);
    memcpy(retval, buffer, len);
    free(buffer);

    return retval;
}