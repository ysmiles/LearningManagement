#ifndef SECURE_H
#define SECURE_H

#include <string>

std::string aesEncript(const std::string &msg);
std::string aesDescript(const std::string &msg);

#endif