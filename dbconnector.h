#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

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

#endif
