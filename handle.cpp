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

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

#define CMDSIZE 128

int handleStudent(int fd, const vector<student> &stu) {
    char buf[CMDSIZE]; // enough to hold ID
    char pass[CMDSIZE];
    bzero(buf, CMDSIZE);
    int sz;

    // TODO
    vector<student> stps;
    loadpasswordInfo(stps);

    while ((sz = read(fd, buf, CMDSIZE))) {
        if (sz < 0)
            errexit("Reading failed: %s\n", strerror(errno));

        // just in case some wrong message
        buf[sz - 1] = '\0';

        string buff = buf;

        if (buff == "END") {
            close(fd);
            return 0;
        }

        // decide if buff is number
        regex idreg("\\d+");
        if (!regex_match(buff, idreg)) {
            buff = "Wrong ID format\n";
        }
        // AUTHENTCATION and send back gpa
        // TODO
        else {
            int id = stoi(buff);
            double gpa = -1;
            for (auto i = stu.begin(); i != stu.end(); ++i) {
                if (id == i->id) {
                    // if id found, break the search
                    pass = i->password;
                    if (pass.empty()) {
                        // Let client enter at least 8 characters long password
                        buff = "Setup Password:\n";
                        write(fd, buff.c_str(), buff.size());
                        // read passward
                        read(fd, buf, CMDSIZE);

                        // TODO
                        strcpy(pass1, buf);
                        string password(pass1);
                        savepassinfo(stu);
                    } else {
                        // Let client enter at least 8 characters long password
                        buff = "Enter Password:\n";
                        write(fd, buff.c_str(), buff.size());
                        // read passward
                        read(fd, buf, CMDSIZE);

                        // TODO
                        strcpy(pass1, buf);
                        string pass2(pass1);

                        // Compare the passwords, if same
                        // then ask for next command to execute.
                        if (pass == pass2) {
                            buff = "How can I help you?";
                            write(fd, buff.c_str(), buff.size());
                            read(fd, buf, CMDSIZE);
                            gpa = i->gpa;
                        } else {
                            buff = "Unauthorised access";
                            write(fd, buff.c_str(), buff.size());
                        }
                    }
                    break;
                }
            }

            // if it is not ID then it has to be a password
            /* strcpy(pass1,buf);
             string pass2(pass1);
           if (pass==pass2)	// Compare the passwords, if same then ask for
           next
           command to execute.
            {
             buff="How can I help you?";
            }
           else{
               buff="Unauthorised access";
               }*/

            // modify sending message
            if (gpa == -1)
                buff = "ID not found";
            else {
                buff = to_string(gpa);
                buff = "GPA " + buff.substr(0, 5);
            }
            buff += '\n';
        }

        if (write(fd, buff.c_str(), buff.size()) < 0)
            errexit("Sending failed: %s\n", strerror(errno));

        // clear the buffer and goto next command
        bzero(buf, CMDSIZE);
    }
    return 0;
}

int handleInstructor(int fd, vector<student> &stu) {
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
            saveInfo(stu);
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
                sqlcmd = "INSERT INTO students VALUE(" + cmd[1] ", '" + cmd[2] +
                         "', '" + cmd[3] + "', " + cmd[4] + ")";
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    ss << "Register succeed" << endl;
                }
            }
            // change id grade
            else if (cmd[0] == "change") {
                sqlcmd = "UPDATE students SET Grade =" + cmd[2] +
                         " WHERE StudentID = " + cmd[1];
                if (dc.exQuery(ss, sqlcmd) == 0) {
                    ss << "Change succeed" << endl;
                }
            }
            // list id
            // del id
            else if (cmd.size() == 2) {
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
                    sqlcmd = "SELECT * FROM students ORDER BY StudentID ASC");
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
                    ss << setw(25) << "list"
                       << "- list all infomation" << endl;
                    ss << setw(25) << "list [id]"
                       << "- show one student's grade" << endl;
                    ss << setw(25) << "del [id]"
                       << "- delete a record" << endl;
                    ss << setw(25) << "stat"
                       << "- show statistics information" << endl;
                    ss << setw(25) << "register [id] [grade]"
                       << "- add an id or change grade" << endl;
                    ss << setw(25) << "change [id] [grade]"
                       << "- change grade" << endl;
                    ss << setw(25) << "END"
                       << "- disconnect with server" << endl;
                }
                buff = ss.str();
            }
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
        auto outlen = buff.size();
        if ((outlen = write(fd, buff.c_str(), outlen)) < 0)
            errexit("Sending failed: %s\n", strerror(errno));

        // might be splitted by system
        while (outlen != buff.size()) {
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