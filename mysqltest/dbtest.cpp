#include "dbconnector.h"

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int main() {
    dbconnector dc;
    string cmd;
    string num = "3";
    cmd = "SELECT EXISTS (SELECT * FROM students WHERE "
          "StudentID = " +
          num + ")";
    cout << cmd << endl;
    if (dc.exQuery(cout, cmd) == 0) {
        auto *res = dc.getresult();
        cout << setw(12) << "Student ID" << setw(15) << "Last Name" << setw(12)
             << "First Name" << setw(15) << "Grade" << endl;
        while (res->next()) {
            /* Access column data by alias or column name */
            /* Access column data by numeric offset, 1 is the first column
             */
            for (auto i = 1; i < 2; ++i)
                cout << setw(12) << res->getString(i);
            // cout << fixed << setprecision(3) << setw(12)
            //  << res->getDouble(4);
            cout << endl;
        }
    }
    return 0;
}