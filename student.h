#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>

class student {
  public:
    friend std::istream &operator>>(std::istream &is, student &s);
    friend std::ostream &operator<<(std::ostream &is, const student &s);
    friend bool operator<(const student &s1, const student &s2);

    student() = default;
    student(const int &a, const double &b) : id(a), gpa(b) {}
    // TODO
    student(const int &c, const std::string &d) : id(c), password(d) {}

  private:
    int id = 0;
    double gpa = 0;
    std::string password;
};

// int loadInfo(std::vector<student> &s);
// int saveInfo(const std::vector<student> &s);
// int loadpasswordInfo(std::vector<student> &s);
// int savepassInfo(const std::vector<student> &s);

#endif