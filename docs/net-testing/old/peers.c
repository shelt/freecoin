#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define MAXMSG  512
#define MESSAGE         "Yow!!! Are we having fun yet?!?"

    int
read_from_client (int filedes)
{
    char buffer[MAXMSG];
    int nbytes;

    nbytes = read (filedes, buffer, MAXMSG);
    if (nbytes < 0)
    {
        /* Read error. */
        printf ("read");
        exit (0);
    }
    else if (nbytes == 0)
        /* End-of-file. */
        return -1;
    else
    {
        /* Data read. */
        fprintf (stdout, "Server: got message: `%s'\n", buffer);
        return 0;
    }
}

    void
write_to_server (int filedes)
{
    int nbytes;

    nbytes = write (filedes, MESSAGE, strlen (MESSAGE) + 1);
    if (nbytes < 0)
    {
        perror ("write");
        exit (0);
    }
}
