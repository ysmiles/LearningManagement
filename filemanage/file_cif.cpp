/* file_cif.c - initw, insertw, deletew, lookupw */

#include <rpc/rpc.h>

#include <stdio.h>

#define RPC_CLNT
#include "file.h"

/* Client-side stub interface routines written by programmer */

extern CLIENT *handle; /* handle for remote procedure	*/

static int *ret; /* tmp storage for return code	*/
static myres *rets;

int checkfilename(char *word) {
    char **arg; /* pointer to argument */
    arg = &word;
    ret = checkfilename_1(arg, handle);
    return ret == 0 ? 0 : *ret;
}

myres fileread(char *word) {
    char **arg; /* pointer to argument */
    arg = &word;
    rets = fileread_1(arg, handle);
    return *rets;
}