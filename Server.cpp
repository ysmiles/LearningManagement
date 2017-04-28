#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "handle.h"
#include "socketstuff.h"
#include "student.h"

// C++ stuff here
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// maximum connection queue length
#define QLEN 10
#define BUFSIZE 2048

void reaper(int);
int handleClient(int fd);

/*------------------------------------------------------------------------
 * main - Concurrent TCP server
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[]) {
    string service = "8000"; // service name or port number
    struct sockaddr_in fsin; // the address of a client
    unsigned int alen;       // length of client's address
    int msock;               // master server socket
    int ssock;               // slave server socket

    switch (argc) {
    case 1:
        break;
    case 2:
        service = argv[1];
        break;
    default:
        errexit("usage: Server [port]\n");
    }
    printf("Use port %s\n", service.c_str());

    // master sock
    msock = passiveTCP(service.c_str(), QLEN);

    (void)signal(SIGCHLD, reaper);

    while (1) {
        alen = sizeof(fsin);
        // slave(client) sock
        ssock = accept(msock, (struct sockaddr *)&fsin, &alen);

        puts("Connection accept.");

        if (ssock < 0) {
            if (errno == EINTR)
                continue;
            errexit("accept: %s\n", strerror(errno));
        }

        switch (fork()) {
        case 0:
            /* child */
            (void)close(msock);
            exit(handleClient(ssock));
        default:
            /* parent */
            (void)close(ssock);
            break;
        case -1:
            errexit("fork: %s\n", strerror(errno));
        }
    }

    return 0;
}

/*------------------------------------------------------------------------
 * reaper - clean up zombie children
 *------------------------------------------------------------------------
 */
/*ARGSUSED*/
void reaper(int sig) {
    int status;
    while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
        /* empty */;
}

/*------------------------------------------------------------------------
 * handleClient - decide which client is connected
 *------------------------------------------------------------------------
 */
int handleClient(int fd) {
    char buf[4]; // here we should get STU or INS
    int sz = read(fd, buf, BUFSIZE);
    if (sz < 0)
        errexit("Reading failed: %s\n", strerror(errno));
    // just in case wrong message
    buf[sz - 1] = '\0';
    printf("Client: %s\n", buf);

    if (strcmp(buf, "STU") == 0)
        return handleStudent(fd);
    else if (strcmp(buf, "INS") == 0)
        return handleInstructor(fd);
    else {
        printf("Wrong client: %s\n", buf);
        return -1;
    }

    return 0;
}