#include "dbconnector.h"
using std::endl;
using std::string;
using std::ostream;
using std::cout;

dbconnector::dbconnector() {
    try {
        /* Create a connection */
        /* Connect to the MySQL test database */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "ys", "ysys");
        db = "studentdb";
        con->setSchema(db);
        stmt = con->createStatement();
    } catch (sql::SQLException &e) {
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

int dbconnector::exQuery(ostream &os, const string &query) {
    try {
        res = stmt->executeQuery(query);
        while (res->next()) {
            os << "MySQL replies: ";
            /* Access column data by alias or column name */
            // os << res->getString("_message") << endl;
            // os << "MySQL says it again: ";
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

dbconnector::~dbconnector() {
    delete res;
    delete stmt;
    delete con;
}