#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "errexit.c"

#include "file.h"
#include <rpc/rpc.h>
#define RMACHINE "localhost" /* name of remote machine	*/
CLIENT *handle;              /* handle for remote procedure	*/

int checkfilename(char *fname);
myres fileread(char *buf);

extern int errno;
int errexit(const char *format, ...);

// engough for holding filename
// for client
char filenameC[64];
FILE *fpC;
// for server
char filenameS[64];
// globle size
int sz;

#define BUFSIZE 1024

/*------------------------------------------------------------------------
 * main - for FILE service
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[]) {
    /* set up connection for remote procedure call  */
    handle = clnt_create(RMACHINE, RFILEPROG, RFILEVERS, "tcp");
    if (handle == 0) {
        printf("Could not contact remote program.\n");
        exit(1);
    }

    // buffer for one line of text
    char buf[BUFSIZE];
    bzero(buf, BUFSIZE);
    // used for size
    int outchars;

    printf("Input file name: ");
    while (fgets(buf, sizeof(buf), stdin)) {
        // insure line null-terminated
        buf[BUFSIZE - 1] = '\0';

        // now buf contains \n
        outchars = strlen(buf);
        strncpy(filenameC, buf, outchars);

        // after this filename does not contain \n
        filenameC[outchars - 1] = '\0';

        // filenameS does not have the postfix
        strncpy(filenameS, filenameC, outchars);

        sz = checkfilename(filenameS);
        printf("Checkname result is (1 means existing) %d\n", sz);

        strcat(filenameC, "_client");
        fpC = fopen(filenameC, "w");

        bzero(buf, BUFSIZE);

        puts("before fileread");

        myres bufres;
        while (1) {
            bufres = fileread(buf);
            sz = bufres.sz;
            strncpy(buf, bufres.content, sz);

            if (sz < 0)
                errexit("Read failed: %s\n", strerror(errno));

            if (strcmp(buf, "Wrong file name") == 0) {
                printf("From server: %s\n", buf);
                sz = 0;
                // break this while
                // go to another command line to read filename (outter while)
                break;
            } else {
                printf("get data %d bytes\n", sz);
                fwrite(buf, sizeof(char), sz, fpC);
                bzero(buf, BUFSIZE);
            }

            if (sz < BUFSIZE) {
                break;
            }
        }

        fclose(fpC);

        // open again and check size.
        fpC = fopen(filenameC, "r");

        fseek(fpC, 0, SEEK_END);
        sz = ftell(fpC);

        if (sz > 0) {
            printf("%s has been successfully got from server.\n", filenameC);
            printf("The size is %d bytes.\n", sz);
        } else {
            if (remove(filenameC) != 0)
                printf("Error: unable to delete the file\n");
            printf("Failed to get %s from server.\n", filenameC);
        }

        printf("Input file name: ");
        fflush(stdout);
    }

    return 0;
}
