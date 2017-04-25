#ifndef SOCKETSTUFF_H
#define SOCKETSTUFF_H

#include <errno.h>

extern int errno;
typedef unsigned short u_short;

int errexit(const char *format, ...);

int connectsock(const char *host, const char *service, const char *transport);
int connectTCP(const char *host, const char *service);

int passivesock(const char *service, const char *transport, int qlen);
int passiveTCP(const char *service, int qlen);

#endif