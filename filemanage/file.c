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

    char filename[128]; // engough for holding filename
    FILE *fp;           // file handler
    char buf[BUFSIZE];  // buffer for one line of text
    int sz;             // used for size

    memset(buf, 0, BUFSIZE);

    printf("Input file name: ");
    while (fgets(filename, sizeof(filename), stdin)) {
        // input abc, filename would be abc\n, sz would be 4
        sz = strlen(filename);
        filename[outchars - 1] = '\0'; // filename strlen would be 3

        sz = checkfilename(filename);

        fp = fopen(filename, "w");

        memset(buf, 0, BUFSIZE);

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
                fwrite(buf, sizeof(char), sz, fp);
                memset(buf, 0, BUFSIZE);
            }

            if (sz < BUFSIZE) {
                break;
            }
        }

        fclose(fp);

        // open again and check size.
        fp = fopen(filename, "r");

        fseek(fp, 0, SEEK_END);
        sz = ftell(fp);

        if (sz > 0) {
            printf("%s has been successfully got from server.\n", filename);
            printf("The size is %d bytes.\n", sz);
        } else {
            if (remove(filename) != 0)
                printf("Error: unable to delete the file\n");
            printf("Failed to get %s from server.\n", filename);
        }

        printf("Input file name: ");
        fflush(stdout);
    }

    return 0;
}
