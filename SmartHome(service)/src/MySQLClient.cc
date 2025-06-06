#include <string.h>
#include "../include/MySQLClient.hpp"

MySQLClient::MySQLClient()
: _conn(nullptr)
, _res(nullptr){
    init();
}

MySQLClient::~MySQLClient(){
    if (_res) {
        mysql_free_result(_res);
        _res = nullptr;
    }

    if (_conn) {
        mysql_close(_conn);
        _conn = nullptr;
    }
}

void MySQLClient::init(){
    _conn = mysql_init(nullptr);
}

void MySQLClient::connect(const string & host,
                          const string & user,
                          const string & passwd,
                          const string & db,
                          unsigned short port){
    if (!mysql_real_connect(_conn, host.c_str(), user.c_str(),
                            passwd.c_str(), db.c_str(), port, nullptr, 0)) {
        printf("%d, %s\n", mysql_errno(_conn), mysql_error(_conn));
        return;
    }
    printf("Mysql connect success\n");
}


bool MySQLClient::writeOperationQuery(const string & sql){
    MYSQL * pconn = _conn;
    if (!_conn) {
        cerr << "Not connected to MySQL server" << endl;
        return false;
    }
    int ret = mysql_real_query(pconn, sql.c_str(), strlen(sql.c_str()));
    if(ret != 0) {
        printf("(%d, %s)\n", mysql_errno(pconn), mysql_error(pconn));
        return false;
    }
    printf("Query OK, %ld row affected\n", mysql_affected_rows(pconn));
    return true;
}

vector<vector<string>> MySQLClient::readOperationQuery(const string &sql){
    vector<vector<string>> result;
    MYSQL * pconn = _conn;
    if (!_conn) {
        cerr << "Not connected to MySQL server" << endl;
        return result;
    }
    int ret = mysql_real_query(pconn, sql.c_str(), strlen(sql.c_str()));
    if(ret != 0) {
        printf("(%d, %s)\n", mysql_errno(pconn), mysql_error(pconn));
        return result;
    }
    _res = mysql_use_result(_conn);
    if(!_res){
        return result;
    }
    //int row = mysql_num_rows(_res);
    int col = mysql_num_fields(_res);
    //printf("result (%d row, %d col)\n\n", row, col);
    MYSQL_FIELD  *filed = NULL;
    vector<string> tmp;
    while((filed = mysql_fetch_field(_res)) != NULL) {
        tmp.push_back(filed->name);
    }
    result.push_back(tmp);
    MYSQL_ROW strRow;
    while((strRow = mysql_fetch_row(_res))!= NULL)  {
        vector<string> tmp;
        for(int i = 0; i < col; ++i) {
            tmp.push_back(strRow[i]);
        }
        result.push_back(tmp);
    }
    return result;
}
