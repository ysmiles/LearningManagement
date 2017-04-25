#include "connectsock.c"

int connectsock(const char *host, const char *service, const char *transport);

int connectTCP(const char *host, const char *service)
/*
 *  Arguments:
 *  host
 *  name of host to which connection is desired
 *  service - service associated with the desired port
 */
{
    return connectsock(host, service, "tcp");
}
