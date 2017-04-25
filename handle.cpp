#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

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
    while ((sz = read(fd, buf, CMDSIZE))) {
        if (sz < 0)
            errexit("Reading failed: %s\n", strerror(errno));

        // just in case some wrong message
        buf[sz - 1] = '\0';

        string buff = buf;

        if (buff == "END") {
            close(fd);
            saveInfo(stu);
            return 0;
        }

        // check format first
        regex idreg("[^\\s]+\\s?(\\d+)?\\s?(\\d+\\.?\\d*)?");
        if (!regex_match(buff, idreg)) {
            buff = "Wrong command, get help with command \"help\"";
        } else {
            // split the command
            istringstream iss(buff);
            vector<string> cmd{istream_iterator<string>(iss), {}};
            cout << "Command get: " << buff << endl;
            buff = ""; // reset buff
            // for (auto i = cmd.begin(); i != cmd.end(); ++i) {
            //     cout << *i << endl;
            // }

            // change id grade
            // register id grade
            if (cmd.size() == 3) {
                int id = stoi(cmd.at(1));
                double gpa = stod(cmd.at(2));
                // find first
                for (auto i = stu.begin(); i != stu.end(); ++i) {
                    if (id == i->id) {
                        i->gpa = gpa;
                        // for checking
                        gpa = 0;
                        // break the search
                        break;
                    }
                }
                ostringstream ss;
                ss << fixed << setprecision(3);
                ss << "ID " << id; // << " Grade " << gpa;
                if (gpa) {
                    stu.push_back(student(id, gpa));
                    ss << " added successful";
                    // sort data
                    sort(stu.begin(), stu.end());
                    // save data to local files
                    saveInfo(stu);
                } else {
                    ss << " changed successful";
                }
                buff = ss.str();
            }
            // list id
            // del id
            else if (cmd.size() == 2) {
                int id = stoi(cmd[1]);
                double gpa = -1;
                for (auto i = stu.begin(); i != stu.end(); ++i) {
                    if (id == i->id) {
                        gpa = i->gpa;
                        if (cmd[0] == "del") {
                            stu.erase(i);
                            // save data to local files
                            saveInfo(stu);
                        }
                        // break the search
                        break;
                    }
                }
                // modify sending message
                if (gpa == -1)
                    buff = "ID not found";
                else {
                    ostringstream ss;
                    ss << fixed << setprecision(3);
                    ss << "ID " << id << " Grade " << gpa;
                    if (cmd[0] == "del")
                        ss << " successfully deleted.";
                    buff = ss.str();
                    // buff = to_string(gpa);
                    // buff = "GPA " + buff.substr(0, 5);
                }
            }
            // list
            // stat
            // help
            else if (cmd.size() == 1) {
                // hold ouput buffer
                ostringstream ss;
                if (cmd[0] == "list") {
                    ss << "ID   Grade" << endl;
                    for (auto i = stu.begin(); i != stu.end(); ++i) {
                        ss << *i << endl;
                    }
                } else if (cmd[0] == "stat") {
                    double min = stu[0].gpa, max = min, ave = 0;
                    ss << setw(8) << "Min" << setw(8) << "Max" << setw(8)
                       << "Average" << endl;
                    for (auto i = stu.begin(); i != stu.end(); ++i) {
                        if (i->gpa < min)
                            min = i->gpa;
                        else if (max < i->gpa)
                            max = i->gpa;
                        ave += i->gpa;
                    }
                    ave = ave / stu.size();
                    ss << fixed << setprecision(3);
                    ss << setw(8) << min << setw(8) << max << setw(8) << ave
                       << endl;
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
        if (write(fd, buff.c_str(), buff.size()) < 0)
            errexit("Sending failed: %s\n", strerror(errno));

        bzero(buf, CMDSIZE);
    }

    return 0;
}