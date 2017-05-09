#ifndef HANDLE_H
#define HANDLE_H

int handleStudent(int fd);
int handleInstructor(int fd);

bool sendmsg(string &buff, int fd);

bool findbystuid(int id);
bool findbyinsid(int id);

#endif