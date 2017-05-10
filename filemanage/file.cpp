#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include <rpc/rpc.h>
#define RMACHINE "localhost" /* name of remote machine	*/
CLIENT *handle;              /* handle for remote procedure	*/

int checkfilename(char *fname);
myres fileread(char *buf);

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
        filename[sz - 1] = '\0'; // filename strlen would be 3

        // also set fpS at server
        if (sz = checkfilename(filename)) {
            // file exist
            myres bufres;

            strcat(filename, "_copy");

            fp = fopen(filename, "w");
            while (1) {
                bufres = fileread(filename);
                sz = bufres.sz;

                // printf("get data %d bytes\n", sz);
                fwrite(bufres.content, sizeof(char), sz, fp);

                if (sz < BUFSIZE)
                    break;
            }
            fclose(fp);

            // open again and check size.
            fp = fopen(filename, "r");
            fseek(fp, 0, SEEK_END);
            sz = ftell(fp);
            printf("%s has been successfully got from server.\n", filename);
            printf("The size is %d bytes.\n", sz);
        } else
            printf("failed to get file %s\n", filename);

        printf("Input file name: ");
        fflush(stdout);
    }

    return 0;
}
