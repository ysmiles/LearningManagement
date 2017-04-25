#ifndef HANDLE_H
#define HANDLE_H

#include "student.h"

int handleStudent(int fd, const std::vector<student> &sts);
int handleInstructor(int fd, std::vector<student> &sts);

#endif