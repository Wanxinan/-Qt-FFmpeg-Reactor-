#include <iostream>
#include <vector>
#include <mysql/mysql.h>
#include <stdlib.h>

using namespace std;

class MySQLClient {

public:

    MySQLClient();

    ~MySQLClient();

    void init();

    void connect(const string & host,
                 const string & user,
                 const string & passwd,
                 const string & db,
                 unsigned short port);
    //写操作封装(INSERT/UPDATE/DELETE)

    bool writeOperationQuery(const string & sql);

    //读操作封装(SELECT)

    vector<vector<string>> readOperationQuery(const string &sql);

private:
    MYSQL * _conn;
    MYSQL_RES * _res;
};
