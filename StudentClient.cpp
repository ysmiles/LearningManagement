#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include "filemanage/filemanage.h"
#include "secure.h"
#include "socketstuff.h"

int TCPecho(const char *host, const char *service);

#define BUFSIZE 1024

/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
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

    TCPecho(host.c_str(), service.c_str());

    exit(0);
}

/*------------------------------------------------------------------------
 * TCPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
int TCPecho(const char *host, const char *service) {

    // buffer for one line of text
    char buf[BUFSIZE + 1];
    /* socket descriptor, read count*/
    int s, n;

    /* characters sent and received */
    int outchars, inchars;

    s = connectTCP(host, service);

    // first identify itself
    (void)write(s, "STU", 4);

    int sz;

    // is password
    bool ispwd = 0;

    // first read
    puts("Enter ID");

    printf("Input: ");
    fflush(stdout);
    while (fgets(buf, sizeof(buf), stdin)) {
        // insure line null-terminated
        outchars = strlen(buf);
        buf[outchars - 1] = '\0';

        string buff = buf;
        if (ispwd) {
            string encrypted = encry(buff);
            (void)write(s, encrypted.c_str(), encrypted.size());
            ispwd = 0;
        } else
            (void)write(s, buf, outchars);

        // close if input END
        if (strcmp(buf, "END") == 0) {
            close(s);
            return 0;
        }

        bzero(buf, BUFSIZE);

        /* read it back */
        while ((sz = read(s, buf, BUFSIZE))) {
            if (sz < 0)
                errexit("socket read failed: %s\n", strerror(errno));
            else {
                printf("%s", buf);
                fflush(stdout);
                buf[sz - 1] = '\0';
                if (strcmp(buf, "allowed") == 0) {
                    // split the command
                    istringstream iss(buff);
                    vector<string> cmd{istream_iterator<string>(iss), {}};
                    if (cmd.at(0) == "download") {
                        vector<string> files(cmd.begin() + 1, cmd.end());
                        freceiver(files, 1);
                    }
                } else if (strcmp(buf, "Enter Password (default: 123456):") ==
                           0) {
                    ispwd = 1;
                }

                bzero(buf, BUFSIZE);
            }
            if (sz < BUFSIZE)
                break;
        }

        fflush(stdout);

        printf("Input: ");
        fflush(stdout);
    }

    return 0;
}
