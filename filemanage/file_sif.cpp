/* file_sif.c - init_1, insert_1, delete_1, lookup_1_svc */

#include <rpc/rpc.h>

#define RPC_SVC
#include "file.h"

/* Server-side stub inteface routines written by hand */

int checkfilename_s(char *);
myres fileread_s(char *);

int *checkfilename_1_svc(char **w, struct svc_req *rqstp) {
    static int retcode;
    retcode = checkfilename_s(*(char **)w);
    return &retcode;
}

myres *fileread_1_svc(char **w, struct svc_req *rqstp) {
    static myres rets;
    rets = fileread_s(*(char **)w);
    return &rets;
}
