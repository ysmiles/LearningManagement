/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "filemanage.h"

static void rfileprog_1(struct svc_req *rqstp, register SVCXPRT *transp) {
    union {
        char *checkfilename_1_arg;
        char *fileread_1_arg;
    } argument;
    char *result;
    xdrproc_t _xdr_argument, _xdr_result;
    char *(*local)(char *, struct svc_req *);

    switch (rqstp->rq_proc) {
    case NULLPROC:
        (void)svc_sendreply(transp, (xdrproc_t)xdr_void, (char *)NULL);
        return;

    case CHECKFILENAME:
        _xdr_argument = (xdrproc_t)xdr_wrapstring;
        _xdr_result = (xdrproc_t)xdr_int;
        local = (char *(*)(char *, struct svc_req *))checkfilename_1_svc;
        break;

    case FILEREAD:
        _xdr_argument = (xdrproc_t)xdr_wrapstring;
        _xdr_result = (xdrproc_t)xdr_myres;
        local = (char *(*)(char *, struct svc_req *))fileread_1_svc;
        break;

    default:
        svcerr_noproc(transp);
        return;
    }
    memset((char *)&argument, 0, sizeof(argument));
    if (!svc_getargs(transp, (xdrproc_t)_xdr_argument, (caddr_t)&argument)) {
        svcerr_decode(transp);
        return;
    }
    result = (*local)((char *)&argument, rqstp);
    if (result != NULL &&
        !svc_sendreply(transp, (xdrproc_t)_xdr_result, result)) {
        svcerr_systemerr(transp);
    }
    if (!svc_freeargs(transp, (xdrproc_t)_xdr_argument, (caddr_t)&argument)) {
        fprintf(stderr, "%s", "unable to free arguments");
        exit(1);
    }
    return;
}

int fsender(bool choice) {
    register SVCXPRT *transp;

    if (choice) {
        pmap_unset(RFILEPROGA, RFILEVERS);

        transp = svcudp_create(RPC_ANYSOCK);
        if (transp == NULL) {
            fprintf(stderr, "%s", "cannot create udp service.");
            exit(1);
        }
        if (!svc_register(transp, RFILEPROGA, RFILEVERS, rfileprog_1,
                          IPPROTO_UDP)) {
            fprintf(stderr, "%s",
                    "unable to register (RFILEPROGA, RFILEVERS, udp).");
            exit(1);
        }

        transp = svctcp_create(RPC_ANYSOCK, 0, 0);
        if (transp == NULL) {
            fprintf(stderr, "%s", "cannot create tcp service.");
            exit(1);
        }
        if (!svc_register(transp, RFILEPROGA, RFILEVERS, rfileprog_1,
                          IPPROTO_TCP)) {
            fprintf(stderr, "%s",
                    "unable to register (RFILEPROGA, RFILEVERS, tcp).");
            exit(1);
        }

        svc_run();
        fprintf(stderr, "%s", "svc_run returned");
        exit(1);
    } else {
        pmap_unset(RFILEPROGB, RFILEVERS);

        transp = svcudp_create(RPC_ANYSOCK);
        if (transp == NULL) {
            fprintf(stderr, "%s", "cannot create udp service.");
            exit(1);
        }
        if (!svc_register(transp, RFILEPROGB, RFILEVERS, rfileprog_1,
                          IPPROTO_UDP)) {
            fprintf(stderr, "%s",
                    "unable to register (RFILEPROGB, RFILEVERS, udp).");
            exit(1);
        }

        transp = svctcp_create(RPC_ANYSOCK, 0, 0);
        if (transp == NULL) {
            fprintf(stderr, "%s", "cannot create tcp service.");
            exit(1);
        }
        if (!svc_register(transp, RFILEPROGB, RFILEVERS, rfileprog_1,
                          IPPROTO_TCP)) {
            fprintf(stderr, "%s",
                    "unable to register (RFILEPROGB, RFILEVERS, tcp).");
            exit(1);
        }

        svc_run();
        fprintf(stderr, "%s", "svc_run returned");
        exit(1);
        /* NOTREACHED */
    }
}
