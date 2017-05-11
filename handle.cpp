#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "dbconnector.h"
#include "handle.h"
#include "socketstuff.h"
#include "student.h"
#include "filemanage.h"

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

#define CMDSIZE 128

int handleStudent(int fd) {
    char buf[CMDSIZE]; // enough to hold ID
    char pass[CMDSIZE];
    bzero(buf, CMDSIZE);
    int sz;
    dbconnector dc;
    string sqlcmd;
    ostringstream ss; // output buffer
    sqlcmd = "SELECT * FROM notifications WHERE Time = "
             "(SELECT  MAX(Time) FROM notifications)";
    if (dc.exQuery(ss, sqlcmd) == 0) {
        auto *res = dc.getresult();
        while (res->next()) {
            // by column
            ss << res->getString(1) << endl;
            ss << res->getString(2) << endl;
        }
        string buff = ss.str();
        if (write(fd, buff.c_str(), buff.size()) < 0)
            errexit("Sending failed: %s\n", strerror(errno));
    }

    // read ID
    while ((sz = read(fd, buf, CMDSIZE))) {
        if (sz < 0)
            errexit("Reading failed: %s\n", strerror(errno));

        // just in case some wrong message
        buf[sz - 1] = '\0';

        string buff = buf;

        // clear the buffer
        bzero(buf, CMDSIZE);

        if (buff == "END") {
            close(fd);
            return 0;
        }

        string sqlcmd;
        ostringstream ss; // output buffer

        //    decide if buff is number
        regex idreg("\\d+");
        if (!regex_match(buff, idreg)) {
            buff = "Wrong ID format\n";
        } else {
            // can be modified by function findbystuid
            // TODO
            string stuid = buff;
            sqlcmd = "SELECT EXISTS (SELECT * FROM students WHERE "
                     "StudentID = " +
                     stuid + ")";
            if (dc.exQuery(ss, sqlcmd) == 0) {
                auto *res = dc.getresult();
                if (res->next()) {
                    // 0 means not exist
                    if (!res->getInt(1)) {
                        ss << "ID not found. Try Again." << endl;
                        buff = ss.str();
                        if (write(fd, buff.c_str(), buff.size()) < 0)
                            errexit("Sending failed: %s\n", strerror(errno));
                        // go to next loop of while
                        continue;
                    }
                }
            }
            // check passwords
            string pwddb;
            sqlcmd = "SELECT Password FROM students WHERE StudentID = ";
            sqlcmd += stuid;
            if (dc.exQuery(ss, sqlcmd) == 0) {
                auto *res = dc.getresult();
                if (res->next()) {
                    // by column
                    pwddb = res->getString(1);
                }
            }
            // Let client enter password
            buff = "Enter Password (default: 123456):\n";
            if (write(fd, buff.c_str(), buff.size()) < 0)
                errexit("Sending failed: %s\n", strerror(errno));

            // read passward
            if ((sz = read(fd, buf, CMDSIZE)) < 0)
                errexit("Reading failed: %s\n", strerror(errno));

            // just in case some wrong message
            buf[sz - 1] = '\0';

            string pwd = buf;

            if (pwd == pwddb) {
                sqlcmd = "SELECT * FROM students WHERE StudentID = ";
                sqlcmd += stuid;
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    auto *res = dc.getresult();
                    ss << setw(12) << "Student ID" << setw(12) << "Last Name"
                       << setw(12) << "First Name" << setw(12) << "Grade"
                       << endl;
                    while (res->next()) {
                        // by column
                        for (auto i = 1; i < 4; ++i)
                            ss << setw(12) << res->getString(i);
                        ss << fixed << setprecision(3) << setw(12)
                           << res->getDouble(4);
                        ss << endl;
                    }
                }
            } else {
                ss << "Wrong password." << endl;
            }
        }
        buff = ss.str();
        if (write(fd, buff.c_str(), buff.size()) < 0)
            errexit("Sending failed: %s\n", strerror(errno));
    }
    return 0;
}

int handleInstructor(int fd) {
    char buf[CMDSIZE];
    bzero(buf, CMDSIZE);
    int sz;
    dbconnector dc;
    string sqlcmd;

    while ((sz = read(fd, buf, CMDSIZE))) {
        if (sz < 0)
            errexit("Reading failed: %s\n", strerror(errno));

        // in case of some wrong message
        buf[sz - 1] = '\0';

        //  C++ string to make things easier
        string buff = buf;

        if (buff == "END") {
            close(fd);
            return 0;
        }

        // split the command
        istringstream iss(buff);
        vector<string> cmd{istream_iterator<string>(iss), {}};
        cout << "Command get: " << buff << endl;
        buff = "";        // reset buff
        ostringstream ss; // output buffer

        if (!cmd.empty()) {
            // register id lastname firstname grade
            if (cmd[0] == "register") {
                sqlcmd = "SELECT EXISTS (SELECT * FROM students WHERE "
                         "StudentID = " +
                         cmd[1] + ")";
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    auto *res = dc.getresult();
                    if (res->next()) {
                        // 0 means not exist
                        if (res->getInt(1)) {
                            ss << "ID already registered. Try Again." << endl;
                            // go to next loop of while
                            buff = ss.str();
                            if (write(fd, buff.c_str(), buff.size()) < 0)
                                errexit("Sending failed: %s\n",
                                        strerror(errno));
                            // go to next loop of while
                            continue;
                        }
                    }
                }
                sqlcmd = "INSERT INTO students(StudentID, LastName, FirstName, "
                         "Grade) VALUE(" +
                         cmd[1] + ", '" + cmd[2] + "', '" + cmd[3] + "', " +
                         cmd[4] + ")";
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    ss << "Register succeed" << endl;
                }
            }
            // change id grade
            else if (cmd[0] == "change") {
                sqlcmd = "SELECT EXISTS (SELECT * FROM students WHERE "
                         "StudentID = " +
                         cmd[1] + ")";
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    auto *res = dc.getresult();
                    if (res->next()) {
                        // 0 means not exist
                        if (!res->getInt(1)) {
                            ss << "ID not found. Try Again." << endl;
                            buff = ss.str();
                            if (write(fd, buff.c_str(), buff.size()) < 0)
                                errexit("Sending failed: %s\n",
                                        strerror(errno));
                            // go to next loop of while
                            continue;
                        }
                    }
                }
                sqlcmd = "UPDATE students SET Grade =" + cmd[2] +
                         " WHERE StudentID = " + cmd[1];
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    ss << "Change succeed" << endl;
                }
            } else if (cmd[0] == "notice") {
                sqlcmd = "INSERT INTO notifications(Time, Content) "
                         "VALUE(CURTIME(), '";
                int sz = cmd.size();
                for (int i = 1; i < sz; ++i) {
                    sqlcmd += cmd[i];
                    sqlcmd += " ";
                }
                sqlcmd += "')";
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    ss << "Notice succeed" << endl;
                }
            }
            // del id
            else if (cmd.size() == 2) {
                sqlcmd = "SELECT EXISTS (SELECT * FROM students WHERE "
                         "StudentID = " +
                         cmd[1] + ")";
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    auto *res = dc.getresult();
                    // 0 means not exist
                    if (res->next()) {
                        if (!res->getInt(1)) {
                            ss << "ID not found. Try Again." << endl;
                            // go to next loop of while
                            buff = ss.str();
                            if (write(fd, buff.c_str(), buff.size()) < 0)
                                errexit("Sending failed: %s\n",
                                        strerror(errno));
                            // go to next loop of while
                            continue;
                        }
                    }
                }
                if (cmd[0] == "list") {
                    sqlcmd = "SELECT * FROM students WHERE StudentID = ";
                    sqlcmd += cmd[1];
                    if (dc.exQuery(ss, sqlcmd) == 0) {
                        auto *res = dc.getresult();
                        ss << setw(12) << "Student ID" << setw(12)
                           << "Last Name" << setw(12) << "First Name"
                           << setw(12) << "Grade" << endl;
                        while (res->next()) {
                            // by column
                            for (auto i = 1; i < 4; ++i)
                                ss << setw(12) << res->getString(i);
                            ss << fixed << setprecision(3) << setw(12)
                               << res->getDouble(4);
                            ss << endl;
                        }
                    }
                } else if (cmd[0] == "del") {
                    sqlcmd = "DELETE FROM students WHERE StudentID = ";
                    sqlcmd += cmd[1];
                    if (dc.exQuery(ss, sqlcmd) == 0) {
                        ss << "delete succeed" << endl;
                    }
                }
            }
            // list
            // stat
            // help
            else if (cmd.size() == 1) {
                if (cmd[0] == "list") {
                    sqlcmd = "SELECT * FROM students ORDER BY StudentID ASC";
                    if (dc.exQuery(ss, sqlcmd) == 0) {
                        auto *res = dc.getresult();
                        ss << setw(12) << "Student ID" << setw(12)
                           << "Last Name" << setw(12) << "First Name"
                           << setw(12) << "Grade" << endl;
                        while (res->next()) {
                            // by column
                            for (auto i = 1; i < 4; ++i)
                                ss << setw(12) << res->getString(i);
                            ss << fixed << setprecision(3) << setw(12)
                               << res->getDouble(4);
                            ss << endl;
                        }
                    }
                } else if (cmd[0] == "stat") {
                    sqlcmd = "SELECT * FROM students WHERE Grade = "
                             "(SELECT  MAX(Grade) FROM students)";
                    ss << "Max grade student" << endl;
                    if (dc.exQuery(ss, sqlcmd) == 0) {
                        auto *res = dc.getresult();
                        ss << setw(12) << "Student ID" << setw(12)
                           << "Last Name" << setw(12) << "First Name"
                           << setw(12) << "Grade" << endl;
                        while (res->next()) {
                            // by column
                            for (auto i = 1; i < 4; ++i)
                                ss << setw(12) << res->getString(i);
                            ss << fixed << setprecision(3) << setw(12)
                               << res->getDouble(4);
                            ss << endl;
                        }
                    }
                    sqlcmd = "SELECT * FROM students WHERE Grade = "
                             "(SELECT  MIN(Grade) FROM students)";
                    ss << "Min grade student" << endl;
                    if (dc.exQuery(ss, sqlcmd) == 0) {
                        auto *res = dc.getresult();
                        ss << setw(12) << "Student ID" << setw(12)
                           << "Last Name" << setw(12) << "First Name"
                           << setw(12) << "Grade" << endl;
                        while (res->next()) {
                            // by column
                            for (auto i = 1; i < 4; ++i)
                                ss << setw(12) << res->getString(i);
                            ss << fixed << setprecision(3) << setw(12)
                               << res->getDouble(4);
                            ss << endl;
                        }
                    }
                    sqlcmd = "SELECT AVG(Grade) FROM students";
                    ss << "Average grade" << endl;
                    if (dc.exQuery(ss, sqlcmd) == 0) {
                        auto *res = dc.getresult();
                        while (res->next()) {
                            // by column
                            ss << fixed << setprecision(4) << res->getDouble(1);
                            ss << endl;
                        }
                    }
                } else if (cmd[0] == "help") {
                    ss << left;
                    ss << "Command examples: " << endl;
                    ss << setw(40) << "list"
                       << "- list all infomation" << endl;
                    ss << setw(40) << "list [id]"
                       << "- show one student's grade" << endl;
                    ss << setw(40) << "del [id]"
                       << "- delete a record" << endl;
                    ss << setw(40) << "stat"
                       << "- show statistics information" << endl;
                    ss << setw(40)
                       << "register [id] [last name] [first name] [grade]"
                       << "- add an id or change grade" << endl;
                    ss << setw(40) << "change [id] [grade]"
                       << "- change grade" << endl;
                    ss << setw(40) << "notice [message]"
                       << "- leave a notification to students." << endl;
                    ss << setw(40) << "END"
                       << "- disconnect with server" << endl;
                }
            }
            buff = ss.str();
        }

        // Since buff has no content
        // other wrong command
        if (buff == "") {
            buff = "Wrong command, get help with command \"help\"";
        }

        // to let client know if the message has been finished
        // because client buffer is 1024
        if (buff.size() % 1024 == 1023)
            buff += " \n";
        else
            buff += '\n';

        // send message
        // next time if it is too large, we can split the string before send
        int outlen = buff.size();
        if ((outlen = write(fd, buff.c_str(), outlen)) < 0)
            errexit("Sending failed: %s\n", strerror(errno));

        // might be splitted by system
        int bufsizebefore = buff.size();
        while (outlen != bufsizebefore) {
            buff = buff.substr(outlen);
            outlen = buff.size();
            if ((outlen = write(fd, buff.c_str(), outlen)) < 0)
                errexit("Sending failed: %s\n", strerror(errno));
        }

        //  reset buf and get next command
        bzero(buf, CMDSIZE);
    }

    return 0;
}

bool findbystuid(const string &id, dbconnector &dc, ostream &ss) {
    string sqlcmd = "SELECT EXISTS (SELECT * FROM students WHERE "
                    "StudentID = " +
                    id + ")";
    if (dc.exQuery(ss, sqlcmd) == 0) {
        auto *res = dc.getresult();
        if (res->next()) {
            // 0 means not exist
            return res->getInt(1);
        }
    }
    // not found
    return 0;
}

bool findbyinsid(const string &id, dbconnector &dc, ostream &ss) {
    string sqlcmd = "SELECT EXISTS (SELECT * FROM instructors WHERE "
                    "insid = " +
                    id + ")";
    if (dc.exQuery(ss, sqlcmd) == 0) {
        auto *res = dc.getresult();
        if (res->next()) {
            // 0 means not exist
            return res->getInt(1);
        }
    }
    // not found
    return 0;
}

bool sendmessage(string &buff, int fd) {
    // to let client know if the message has been finished
    // because client buffer is 1024
    if (buff.size() % 1024 == 1023)
        // add 2 chars
        buff += " \n";
    else
        // add 1 new line character
        buff += '\n';

    // send message
    // next time if it is too large, we can split the string before send
    int outlen = buff.size();
    if ((outlen = write(fd, buff.c_str(), outlen)) < 0)
        errexit("Sending failed: %s\n", strerror(errno));

    // might be splitted by system
    int bufsizebefore = buff.size();
    while (outlen != bufsizebefore) {
        buff = buff.substr(outlen);
        outlen = buff.size();
        if ((outlen = write(fd, buff.c_str(), outlen)) < 0)
            errexit("Sending failed: %s\n", strerror(errno));
    }
    // succeed
    return 1;
}