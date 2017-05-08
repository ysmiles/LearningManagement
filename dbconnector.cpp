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
        // con = driver->connect("tcp://127.0.0.1:3306", "ys", "ysys");
        // TODO
        // Used for remote database test
        con = driver->connect("tcp://162.243.136.123:3306", "ys", "ys011822sy096");
        db = "studentdb";
        con->setSchema(db);
        stmt = nullptr;
    } catch (sql::SQLException &e) {
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

int dbconnector::exQuery(ostream &os, const string &query) {
    try {
        if (stmt != nullptr)
            delete stmt;
        stmt = con->createStatement();
        res = stmt->executeQuery(query);
    } catch (sql::SQLException &e) {
        if (e.getErrorCode() == 0)
            return 0;
        os << "# ERR: " << e.what();
        os << " (MySQL error code: " << e.getErrorCode();
        os << ", SQLState: " << e.getSQLState() << " )" << endl;
        return -1;
    }

    return 0;
}

sql::ResultSet *dbconnector::getresult() { return res; }

dbconnector::~dbconnector() {
    delete res;
    delete stmt;
    delete con;
}