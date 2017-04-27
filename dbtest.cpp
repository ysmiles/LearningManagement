#include "dbconnector.h"

#include <iostream>
#include <string>
using namespace std;
int main() {
    dbconnector dc;
    string cmd;
    while (getline(cin, cmd))
        dc.exQuery(cout, cmd);
    return 0;
}