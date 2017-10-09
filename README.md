## 1. Introduction

This repository is originally used for Yu Shu's record of 2017-Spring CMPE-207 project.

It is a network based learning management system. (something like Canvas or Blackboard).

The overview

![img](/image20.png)

At this stage, we can divide our whole system as 3 key part.

The general message transfer server. This is the core.

The MySQL database system to save information. This database is running remotely at a VPS (virtual private server) provided by DigitalOcean.

The RPC file handling subsystem for file transfer and store. (currently, it runs locally as the core server. But in the future, it could be implemented at other machine to achieve higher level of distribution.

When the Server starts to run, it first fork once to create a RPC file sender (version 1) to handle files sending. Client can call RPC receiver to download files.

At the client side, when we open a client, it also fork once to run a RPC sender (version 2). When an instructor want to upload files, actually the client would send a command to server and let server download files from client side.

And for each instructor the file should be automatically uploaded to the specific folder.
A Network Based Distributed Learning Management System

________________
________________


### Step 1
Goal: build connection.

Server: 
1. Has only grades information. 
2. Hold a text file for the ID number and grades (just a total GPA number for now.)
3. Send a message like "ID: 01, GPA: 4.000" to client.

Client: 
1. Can identify itself to the server and ask for information.
2. Students can ask for one grade.
3. Instructors can ask server for any grades (for a specific student or all).
4. No security feature at this stage.
5. Instructors can register students (an ID) into the system and after that the ID can connect the server.

### Step 2
Goal: enrich features.

Enrich the data information, try to involve some basic database feature.
1. Name, ID number, password, course name, grades, GPA, etc.
Simple tables, but we have implement the connection between our server with MySQL database server. Also we have implemented all previous commands  with the database.
1. Also file attachments can be associated and shared with some specific IDs.
Under working as a choosable feature.
1. For example, for the one specific file HW1.txt, the server should have only one copy and relate it to instructor and the student.

Notification feature
1. Instructors can leave notifications/homeworks.
2. When a student client connects, he/she can see that notification.


Basic security feature
        A password (not encrypted) for each ID.

### Step 3
Goal: more feature and refining.

Changed all stuff to C++.
        Make everything more uniform.

Further enrich database content and functions.
1. Save more details in the database.
2. More analysis/application feature to give help with students study and communication with instructors.

RPC based file transfer subsystem.
1. Instructor can upload homeworks as well as other course materials to server.
2. Student can download files from server.
3. File transfer through Remote Procedure Call (RPC).
4. Can download or upload multiple files by one command.

Reliable security feature:
1. Channel encryption. (AES, fixed key inside program at this stage.)
2. Database encryption (for password, in remote MySQL database).

## 2. Design and implementation
2.1        Evolution of the whole system

2.1.1        Simple implementation by saving information in a text file
  

Basically, at this stage, we want to make everything simple and build the basic structure of the whole program, so we can add more details later.
As the figure before, our server can interact with different client, StudentClient and InstructorClient. For a student, only grade of an ID can be provided (because now there is no password actually a student can get other student’s grade if he/she knows the ID of the other student). For an instructor who use InstructorClient, he/she can not only access the data but also modify the data, including adding, removing, changing modification. Also he/she can get (basic) statistics information about the grades.

2.1.2        Involving MySQL Database

1. At this stage, a database interaction between our server and the MySQL database has been added.
2. Same as before, shown in the figure above, our server can interact with different client, StudentClient and InstructorClient.
3. For a student, only grade of an ID can be provided. Also we have implement a simple password feature to control the authentication. Furthermore, when students login through the StudentClient, he/she can see the most recent notification.
4. For an instructor who use InstructorClient, he/she can not only access the data but also modify the data, including adding, removing, changing modification. Also he/she can get (basic) statistics information about the grades. Now the instructor can also leave notification to students.

2.1.3        Introducing RPC file transfer subsystem and security feature
  

At this stage, we can divide our whole system as 3 key part.
1. The general message transfer server. This is the core.
2. The MySQL database system to save information. This database is running remotely at a VPS (virtual private server) provided by DigitalOcean.
3. The RPC file handling subsystem for file transfer and store. (currently, it runs locally as the core server. But in the future, it could be implemented at other machine to achieve higher level of distribution.
When the Server starts to run, it first fork once to create a RPC file sender (version 1) to handle files sending. Client can call RPC receiver to download files.
At the client side, when we open a client, it also fork once to run a RPC sender (version 2). When an instructor want to upload files, actually the client would send a command to server and let server download files from client side.
And for each instructor the file should be automatically uploaded to the specific folder.
Also this version kept all the feature of previous ones.

2.2        Data structure and methods design

2.2.1        student.h

This simple student class here same as report #1.
In current version, we actually bypassed this class (all information is saved in MySQL database, all course materials files are saved locally at server side). But it could be optimized and useful in the future feature implementation to get easy inputs/outputs of students information.
Overloaded operators has been declared (and defined at other place) to support IO process, sorting process, etc. or make them easier. 

```C++
class student {
 public:
   friend std::istream &operator>>(std::istream &is, student &s);
   friend std::ostream &operator<<(std::ostream &is, const student &s);
   friend bool operator<(const student &s1, const student &s2);

   student() = default;
   student(const int &a, const double &b) : id(a), gpa(b) {}

   int id = 0;
   double gpa = 0;
};
```

2.2.2        Records in database

Several regular records of students are stored as the following figure shows.

For demonstration purpose, ID 2 and 5 have their password unencrypted on purpose. ID 1’s password has been encrypted by calling MySQL build-in aes encryption function, such as AES_ENCRYPT('123456', UNHEX(SHA2('ysys',512))).
Course grade -1 has special meaning that this student did not chose this course.

2.2.3        Others

Please refer to section 2.6 for dbconnector, 2.7 for RPC file sender and receiver, and 2.8 for string encryption and decryption.

2.3        Command design

The command is roughly same as previous report.
But the core part has been changed.
Now we get information from MySQL database through the communication between our server with the database.
Also more details will be shown in the result. Please refer to the demonstration part.

2.3.1        Student command

```
check 
        Get the grade of this ID.
Download [file1] [file2] ... 
        Download multiple files.
```

2.3.2        Instructor command

```
list
        List all IDs and grades of this course.
list [IDnumber]
        Get the grade of a specific student.
stat
        Get grades statistic information.
register [ID number] [last name] [first name]
        Register a new student to this course.
Change [ID number] [grade]
        Change the score of a student, of this specific course.
notice [message]
        Leave a message to students.
upload [file1] [file2] ...
        Upload multiple files.
```

2.3.3        Administrator command

All instructor commands. Actually the administrator and instructors use a same client. The administrator has id = 0.

```
del [IDnumber]
        Delete a student’s record.
```

2.3.4        General command for all users

```
        END
                Terminate the connection with server.
        help
                Get help with commands.
```

2.4        Connection and multitasking

In a word, we implement multitasking by one client a process. On the one hand, multi processing is easy to implement. On the other hand, our main program is relatively simple, so it won’t take too much resources when forking processes.
At server side, first we create a TCP socket. Then every time a client is connected, the server will load (the latest) data from the text file (StudentsInfo.txt) to memory in a vector container from C++ standard library.
The parent process will create a child process to handle the interaction between the client, and pass the vector (reference) to function handleClient. Then the parent process will go to the next loop to wait for next connection. The vector used will go out of scope and release to save memory.
Here is the part of the codes. For more details please refer to the source code.


```C++
   while (1) {
       alen = sizeof(fsin);
       // slave(client) sock
       ssock = accept(msock, (struct sockaddr *)&fsin, &alen);

       puts("Connection accept.");

       if (ssock < 0) {
           if (errno == EINTR)
               continue;
           errexit("accept: %s\n", strerror(errno));
       }

       switch (fork()) {
       case 0:
           /* child */
           (void)close(msock);
           exit(handleClient(ssock, sts));
       default:
           /* parent */
           (void)close(ssock);
           break;
       case -1:
           errexit("fork: %s\n", strerror(errno));
       }
   }
```

2.5        Utilities used

The C source files are compiled with gcc and C++ source files with g++ (under -std=c++11). As the final version, whole files are C++.
Basic Linux socket APIs (socket, connect, etc.) and file IO (read, write, etc.) APIs have been used.
To make things easier, some C++ features are used, including regular expression, vector container, string, string stream, etc.
An example of C++ string and string stream usage are shown as follows. For more details please go to the source codes.


```C++
    stringstream ss;
    // ss << “content” << endl;
    string buff = ss.str();
    // send message
    // (next time) if it is too large, we can split the string before send
    if (write(fd, buff.c_str(), buff.size()) < 0)
        errexit("Sending failed: %s\n", strerror(errno));
```

2.6 Implementation of database

2.6.1 dbconnector.h (database connector)

Based on the MySQL official document and examples, we have designed a class named dbconnector. This class encapsulate one further layer compared to the official MySQL C++ connector API.

When compiling, -lmysqlcppconn need to be added. Please refer to Makefile.

Taking advantage of this class along with its implementation. We can easily get communication with the database.
Here are explanations of the members.

```C++
int exQuery(std::ostream &os, const std::string &query)
        Public member function to get input and let the execute the SQL command.
sql::Driver *driver
        Driver object (pointer) to initialize the connector.
        Sample usage:
driver = get_driver_instance();
sql::Connection *con
Connection (object) to initialize the connection.
```

Sample usage:
```C++
    con = driver->connect("tcp://hostname:3306", "user", "password");
    sql::Statement *stmt; // Used for holding SQL command.
    sql::ResultSet *res; // Used for fetching result from database.
    std::string db; // Name of database. In fixed as “studentdb”.
```

The following is the header dbconnector.h. For detailed implementation, please refer to the source code.

```C++
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <string>

class dbconnector {
 public:
   dbconnector();

   int exQuery(std::ostream &os, const std::string &query);

   sql::ResultSet *getresult();

   ~dbconnector();

 private:
   sql::Driver *driver;
   sql::Connection *con;
   sql::Statement *stmt;
   sql::ResultSet *res;
   std::string db;
};
```

2.6.2        Connect to MySQL database

Here is an example of our code about how to implement the database feature.
The following code is get the most recent notifications from instructor.

Here, dc is the dbconnector class. We use the SQL command “SELECT * FROM notifications WHERE Time = (SELECT  MAX(Time) FROM notifications)” to fetch the most recent timestamp entry, and then send it back to client (fd).

```C++
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
```

2.7 RPC file transfer subsystem

A remote procedure call (RPC) is when a computer program causes a procedure (subroutine) to execute in another address space (commonly on another computer on a shared network), which is coded as if it were a normal (local) procedure call, without the programmer explicitly coding the details for the remote interaction. (ref:https://en.wikipedia.org/wiki/Remote_procedure_call)

File transfer is a special usage of RPC. Here first, we used rpcgen at Linux system to generate the rpc structures. The following is the original file.x file.

```C++
const BUFSIZE = 1024; /* maximum length of a command or word */
typedef string mystring<BUFSIZE>;

struct myres { /* structure declared here to */
        int sz; /* illustrate how rpcgen builds XDR */
        mystring content; /* routines to convert structures. */
};


program RFILEPROG { /* name of remote program (not used) */
        version RFILEVERS { /* declaration of version (see below) */
            int CHECKFILENAME(string) = 1; /* first procedure in this program */
            myres FILEREAD(string) = 2; /* second procedure in this program */
        } = 1; /* definition of the program version  */
} = 0x30090949; /* remote program number (must beunique)  */
```

The struct myres is the special return value data structure we defined for the return value from RPC remote end (server, i.e. file sender).

Then besides general file transfer implementation, we also modified the client and server program, as well as file.h file.

The key part is define 2 (or more) RFILEPROG number, to allow different uploading and downloading from both sides at same time.


```C++
// download files from server
#define RFILEPROGA 0x30090949
// upload files to server
#define RFILEPROGB 0x30090950
```

On the other hand, we modified the RPC client and server main functions, to let them be called by our server and client program.

```C++
int freceiver(const vector<string> &fnames, bool choice);
int fsender(bool choice);
```

For more details in this part, please refer to our source code filemanage folder.

2.8 Security

As we mentioned at section 2.2.2, passwords in database can be encrypted. So even though our database were hacked by others, they cannot see the password.
In this section, we will give more introduction about channel encryption.

The libcrypto library within OpenSSL provides functions for performing symmetric encryption and decryption operations across a wide range of algorithms and modes. Following the guide from openssl.org (please see reference), we performed a simple encryption and corresponding decryption operation.

We defined 2 function for encryption and decryption.


```C++
std::string encry(const std::string &st);
…
std::string dencry(const std::string &st);
```

They called the library defined functions, like EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv), etc. to encrypt and decrypt strings.
To compile those files, -lcrypto should be added in the compile command.
Then just implement those functions before sending password and by receiving password, we created a safe channel.

For more details, please refer to secure.h and secure.cpp.

## 3. Demonstration

3.1        Compiling

First compile in filemanage folder
  
Then compile in root folder
  
3.2        Student

Log in:

We can also see the notification at the beginning.

Check, this student only chose CourseA before.

help

Download files (including wrong file test)

After downloading

END
  
3.3 Instructor
Log in (instructor of CourseA):

List all the student’s grades (of a specific course):

Register and give a grade by using change

List a specific student info

Notification (please refer to Student demo to see this notification):

Upload course materials:

The files will be uploaded automatically to CourseA folder

END

3.4 Administrator (instructorID = 0)
Log in

List info (show total grade)

Delete a student record

Statistic information

Help

3.5 Security demonstration by wireshark

For demonstration purpose, we did not implement encryption for instructor at this time.

By using wireshark, when we log in, we can fetch the password of this user, 123456.

From server “enter password”

From client, the password is 123456.

For student, because we implement AES encryption. Wireshark cannot fetch anything like the one before, but only the “enter password” and “welcome!” message.

## 4. Discussion
4.1        Possible problems at this stage
1. Our program is still not secure enough. Only passwords are encrypted when transfer and store.
2. The content related features could be further more enriched.
3. Programming could be improved to be more structuralized and maintainable. 

4.2        Possible following work
1. Encapsulate some repeated codes to functions and classes. 
2. Add more content related features, such as enrolling courses, add courses (got those ideas from our peers’ presentations).
3. Improve encryption coverage of information storage and transfer.
4. For better usage, GUI should be considered. Webpage based GUI could be better because the ability of cross-platform.

4.3        Challenges and problems we have met 
1. Learning to write makefile.
2. Segment faults by misuse of pointers.
3. Learning to use MySQL.
4. Learning encryption and decryption.
5. Modifying the files generated from rpcgen.

## References
* Internetworking with TCP/IP Vol. 3, Client-Server programming and applications,
Comer and Stevens, Linux/POSIX Sockets version, ISBN: 0-13-032071-4, 2001.
* www.cplusplus.com
* en.cppreference.com
* https://dev.mysql.com/doc/connector-cpp/en/
* https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

### 5. Source Code List
In root folder

```
Dbconnector.h
        Declare the class dbconnector.
Handle.h
        Declare several functions used by server to handle clients.
Secure.h
        Secure feature header.
Socketstuff.h
        Declare several functions as well as some include headers.

Connectsock.cpp
        Handle basic socket connection
Errexit.cpp
        Handle errors.
Passivesock.cpp
        Handle server socket creation and binding.
Server.cpp
        Server main function.
connectTCP.cpp
        Connect by TCP.
Handle.cpp
        Functions calling by server to handle different clients.
passiveTCP.cpp
        Used for TCP socket binding.
StudentClient.cpp
        Main function for StudentClient.
Dbconnector.cpp
        Implementation of dbconnector
InstructorClient.cpp
        Main function for InstructorClient, used by general instructor and administratior.
Secure.cpp
        Implementation of encryption and decryption.
```

In filemanage folder:

```
File.h
        Used locally for RPC establish.
Filemanage.h
        Used by outside programs to include the sender and receiver function.


File_cif.cpp
        RPC client interface stub.
File_sif.cpp
        RPC server interface stub.
File_xdr.cpp
        For xdr, generated by rpcgen.
Fsender.cpp
        Main fsender function.
File_clnt.cpp
        Implement functions called by freceiver.
File_srp.cpp
        Main loginc of fsender (RPC server).
Freceiver.cpp
        Main freceiver function (RPC client).
```