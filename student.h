#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <vector>

class student {
  public:
    friend std::istream &operator>>(std::istream &is, student &s);
    friend std::ostream &operator<<(std::ostream &is, const student &s);
    friend bool operator<(const student &s1, const student &s2);

    student() = default;
    student(const int &a, const double &b) : id(a), gpa(b) {}
    // next time the following would be private member
    int id = 0;
    double gpa = 0;
};

int loadInfo(std::vector<student> &s);
int saveInfo(const std::vector<student> &s);

#endif