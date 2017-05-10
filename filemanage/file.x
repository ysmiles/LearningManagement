/* file.x */

/* RPC declarations for dictionary program */

const BUFSIZE = 1024; /* maximum length of a command or word	*/
typedef string mystring<BUFSIZE>;

struct myres {   /* unused structure declared here to	*/
    int sz;  /* illustrate how rpcgen builds XDR	*/
    mystring content; /* routines to convert structures.	*/
};

/*------------------------------------------------------------------------
 * RDICTPROG - remote program that provides insert, delete, and lookup
 *------------------------------------------------------------------------
 */
program RFILEPROG {                     /* name of remote program (not used)	*/
    version RFILEVERS {                 /* declaration of version (see below)	*/
        int CHECKFILENAME(string) = 1; /* first procedure in this program */
        myres FILEREAD(string) = 2;      /* second procedure in this program	*/
    } = 1; /* definition of the program version	*/
} = 0x30090949; /* remote program number (must beunique)	*/