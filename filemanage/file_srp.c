#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "errexit.c"

#include "file.h"
#include <rpc/rpc.h>

extern int errno;
int errexit(const char *format, ...);

// for server
static FILE *fpS;

#define BUFSIZE 1024

int checkfilename(char *fname) {
    puts("Check name start");
    printf("File %s\n", fname);
    if (access(fname, R_OK) != -1) {
        puts("File exits");
        fpS = fopen(fname, "r");
        return 1;
    } else {
        puts("File does not exist");
        fpS = NULL;
        return 0;
    }
}

myres fileread(char *buf) {
    // char *result = (char *)malloc(BUFSIZE);
    char result[BUFSIZE];
    memset(result, '\0', BUFSIZE);
    int s;
    // check if the file exist locally
    if (fpS == NULL) {
        printf("(Serverside) Wrong file name.\n");
        sprintf(result, "Wrong file name");
        s = 16;
    } else {
        if ((s = fread(result, sizeof(char), BUFSIZE, fpS)) < BUFSIZE)
            fclose(fpS);
    }
    puts("It's about to send result back.");
    myres res; 
    res.sz = s;
    res.content = result;
    return res;
}