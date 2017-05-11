#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include <rpc/rpc.h>

// for server
static FILE *fpS;

#define BUFSIZE 1024

int checkfilename_s(char *fname) {
    printf("File %s", fname);
    fflush(stdout);
    if (access(fname, R_OK) != -1) {
        fpS = fopen(fname, "r");
        return 1;
    } else {
        puts(" does not exist");
        fpS = nullptr;
        return 0;
    }
}

myres fileread_s(char *buf) {
    // char *result = (char *)malloc(BUFSIZE);
    char result[BUFSIZE];
    memset(result, '\0', BUFSIZE);
    int s;
    // check if the file exist locally
    if (fpS == nullptr) {
        printf("(Serverside) Wrong file name.\n");
        sprintf(result, "Wrong file name");
        s = 16;
    } else {
        if ((s = fread(result, sizeof(char), BUFSIZE, fpS)) < BUFSIZE)
            fclose(fpS);
    }
    // printf("send data %d bytes\n", s);

    myres res;
    res.sz = s;
    res.content = result;
    printf("\n%d bytes to be sent by RPC\n", s);

    return res;
}