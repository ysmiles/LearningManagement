#ifndef HANDLE_H
#define HANDLE_H

#include <iostream>
#include <string>
#include "dbconnector.h"

using std::ostream;
using std::string;

int handleStudent(int fd);
int handleInstructor(int fd);

bool sendmessage(string &buff, int fd);

bool findbystuid(const string &id, dbconnector &dc, ostream &os);
bool findbyinsid(const string &id, dbconnector &dc, ostream &os);

#endif