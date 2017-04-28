#include "student.h"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

istream &operator>>(istream &is, student &s) {
    is >> s.id >> s.gpa;
    return is;
}

// istream &operator>>(istream &is, student &s) {
//     is >> s.id >> s.password;
//     return is;
// }

ostream &operator<<(ostream &os, const student &s) {
    os << fixed << setprecision(3);
    os << setw(2) << s.id;
    os << setw(8) << s.gpa;
    return os;
}

// ostream &operator<<(ostream &os, const student &s) {
//     os << fixed << setprecision(3);
//     os << setw(2) << s.id;
//     os << setw(14) << s.password;
//     return os;
// }

bool operator<(const student &s1, const student &s2) {
    return s1.id < s2.id;
}

// int loadInfo(vector<student> &s) {
//     fstream fs;
//     fs.open("StudentsInfo.txt");
//     student temp;
//     while (fs >> temp) {
//         s.push_back(temp);
//     }
//     fs.close();
//     return 0;
// }

// int saveInfo(const vector<student> &s) {
//     fstream fs;
//     fs.open("StudentsInfo.txt");
//     for (auto i = s.begin(); i != s.end(); ++i) {
//         fs << *i << endl;
//     }
//     fs.close();
//     return 0;
// }

// int loadpasswordInfo(vector<student> &s1) {
//     fstream fs1;
//     fs1.open("StudentPasswordInfo.txt");
//     student temp1;
//     while (fs1 >> temp1) {
//         s1.push_back(temp1);
//     }
//     fs1.close();
//     return 0;
// }

// int savepassInfo(const std::vector<student> &s1) {
//     fstream fs1;
//     fs1.open("StudentPasswordInfo.txt");
//     for (auto j = s1.begin(); j != s1.end(); ++j) {
//         fs1 << *j << endl;
//     }
//     fs1.close();
//     return 0;
// }
