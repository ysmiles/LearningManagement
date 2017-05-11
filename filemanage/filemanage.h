#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <string>
#include <vector>

#include "file.h"
#include <memory.h>
#include <netinet/in.h>
#include <rpc/pmap_clnt.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define RMACHINE "localhost" /* name of remote machine	*/

#ifndef SIG_PF
#define SIG_PF void (*)(int)
#endif

#define BUFSIZE 1024

using std::string;
using std::vector;

int checkfilename(char *fname);
myres fileread(char *buf);

int freceiver(const vector<string> &fnames, bool choice);
int fsender(bool choice);

#endif