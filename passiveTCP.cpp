#include "socketstuff.h"

/*------------------------------------------------------------------------
 * * passiveTCP - create a passive socket for use in a TCP server
*------------------------------------------------------------------------
*/
int passiveTCP(const char *service, int qlen)
/*
 * * Arguments:
 * *
 * service - service associated with the desired port
 * * qlen - maximum server request queue length
 * */
{
    return passivesock(service, "tcp", qlen);
}
