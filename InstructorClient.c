#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "connectTCP.c"

extern int errno;
int Interact(const char *host, const char *service);
int errexit(const char *format, ...);
int connectTCP(const char *host, const char *service);

#define BUFSIZE 1024

/*------------------------------------------------------------------------
 * main - TCP client for Instructor
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[]) {
    // host to use if none supplied
    char *host = "localhost";
    // default service port
    char *service = "8000";

    switch (argc) {
    case 1:
        printf("Using default host: %s:%s\n", host, service);
        break;
    case 3:
        service = argv[2];
    /* FALL THROUGH */
    case 2:
        host = argv[1];
        printf("Using host: %s:%s\n", host, service);
        break;
    default:
        fprintf(stderr, "usage: Client [host] [port]\n");
        exit(1);
    }

    // Interact with server
    Interact(host, service);

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

    printf("Input command: ");
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

        printf("Input command: ");
        fflush(stdout);
    }

    return 0;
}
