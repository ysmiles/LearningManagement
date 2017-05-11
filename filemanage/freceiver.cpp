#include "filemanage.h"

CLIENT *handle; /* handle for remote procedure	*/

// file receiver
int freceiver(const vector<string> &fnames, bool choice) {
    /* set up connection for remote procedure call  */
    if (choice)
        handle = clnt_create(RMACHINE, RFILEPROGA, RFILEVERS, "tcp");
    else
        handle = clnt_create(RMACHINE, RFILEPROGB, RFILEVERS, "tcp");

    if (handle == 0) {
        printf("Could not contact remote program.\n");
        exit(1);
    }

    FILE *fp; // file handler
    char filename[128];

    int sz; // used for size

    for (auto fi : fnames) {
        strncpy(filename, fi.c_str(), fi.size() + 1);
        // also set fpS at server
        if ((sz = checkfilename(filename))) {
            // file exist
            myres bufres;

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
            printf("%s (%d bytes) downloaded through RPC\n", filename, sz);
        } else
            printf("failed to get file %s\n", filename);
    }

    return 0;
}
