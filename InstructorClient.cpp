#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>
using std::string;

#include "socketstuff.h"
#include "filemanage/filemanage.h"

int Interact(const char *host, const char *service);

#define BUFSIZE 1024

/*------------------------------------------------------------------------
 * main - TCP client for Instructor
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[]) {
    // host to use if none supplied
    string host = "localhost";
    // default service port
    string service = "8000";

    switch (argc) {
    case 1:
        printf("Using default host: %s:%s\n", host.c_str(), service.c_str());
        break;
    case 3:
        service = argv[2];
    /* FALL THROUGH */
    case 2:
        host = argv[1];
        printf("Using host: %s:%s\n", host.c_str(), service.c_str());
        break;
    default:
        fprintf(stderr, "usage: Client [host] [port]\n");
        exit(1);
    }

    // Open a file sender
    if (fork() == 0) {
        // child
        return fsender(0);
    }

    // Interact with server
    Interact(host.c_str(), service.c_str());

    exit(0);
}

/*------------------------------------------------------------------------
 * Interact - send command to server and print out the message from server
 *------------------------------------------------------------------------
 */
int Interact(const char *host, const char *service) {
    // buffer for one line of text
    char buf[BUFSIZE];
    bzero(buf, BUFSIZE);
    /* socket descriptor, read count*/
    int sock, sz;

    /* characters sent and received */
    int outchars;

    sock = connectTCP(host, service);

    (void)write(sock, "INS", 4);

    printf("Input: ");
    fflush(stdout);
    while (fgets(buf, sizeof(buf), stdin)) {

        outchars = strlen(buf);
        buf[outchars - 1] = '\0';

        (void)write(sock, buf, outchars);

        // close if input END
        if (strcmp(buf, "END") == 0) {
            close(sock);
            return 0;
        }

        bzero(buf, BUFSIZE);

        /* read it back */
        while ((sz = read(sock, buf, BUFSIZE))) {
            if (sz < 0)
                errexit("socket read failed: %s\n", strerror(errno));
            else {
                printf("%s", buf);
                fflush(stdout);
                bzero(buf, BUFSIZE);
            }
            if (sz < BUFSIZE)
                break;
        }

        printf("Input: ");
        fflush(stdout);
    }

    // here the socket has already been closed

    return 0;
}
