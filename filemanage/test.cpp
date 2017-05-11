#include "filemanage.h"
#include <iostream>

using namespace std;

int main() {
    int choice;
    cin >> choice;
    vector<string> v1 = {"2/file1_test", "2/file2_test"};
    vector<string> v2 = {"file3_test", "file4_test"};

    if (choice == 1)
        return fsender(0);
    else if (choice == 2)
        return fsender(1);
    else if (choice == 3)
        return freceiver(v1, 0);
    else if (choice == 4)
        return freceiver(v2, 1);

    return 0;
}