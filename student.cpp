#include "student.h"
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

istream &operator>>(istream &is, student &s) {
    is >> s.id >> s.gpa;
    return is;
}

ostream &operator<<(ostream &os, const student &s) {
    os << fixed << setprecision(3);
    os << setw(2) << s.id;
    os << setw(8) << s.gpa;
    return os;
}

bool operator<(const student &s1, const student &s2) { return s1.id < s2.id; }

int loadInfo(vector<student> &s) {
    fstream fs;
    fs.open("StudentsInfo.txt");
    student temp;
    while (fs >> temp) {
        s.push_back(temp);
    }
    fs.close();
    return 0;
}

int saveInfo(const vector<student> &s) {
    fstream fs;
    fs.open("StudentsInfo.txt");
    for (auto i = s.begin(); i != s.end(); ++i) {
        fs << *i << endl;
    }
    fs.close();
    return 0;
}