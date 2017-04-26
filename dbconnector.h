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
    dbconnector() {
        try {
            /* Create a connection */
            /* Connect to the MySQL test database */
            driver = get_driver_instance();
            con = driver->connect("tcp://127.0.0.1:3306", "ys", "ysys");
            con->setSchema(db);
            stmt = con->createStatement();
        } catch (sql::SQLException &e) {
            cout << "# ERR: " << e.what();
            cout << " (MySQL error code: " << e.getErrorCode();
            cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        }
    }

    int exQuery(std::ostream &os, const std::string &query) {
        try {
            res = stmt->executeQuery(query);
            while (res->next()) {
                os << "MySQL replies: ";
                /* Access column data by alias or column name */
                os << res->getString("_message") << endl;
                os << "MySQL says it again: ";
                /* Access column data by numeric offset, 1 is the first */
                os << res->getString(1) << endl;
            }
        } catch (sql::SQLException &e) {
            os << "# ERR: " << e.what();
            os << " (MySQL error code: " << e.getErrorCode();
            os << ", SQLState: " << e.getSQLState() << " )" << endl;
        }

        return 0;
    }
    ~dbconnector() {
        delete res;
        delete stmt;
        delete con;
    }

  private:
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    std::string db = "studentdb";
};

#endif
