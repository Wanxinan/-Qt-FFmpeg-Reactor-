#include <string>
#include <vector>
#include "MySQLClient.hpp"
#include "memory"

using namespace std;

class PasswordQtValidator {
private:
    shared_ptr<MySQLClient> _mysqlClient;

public:
    PasswordQtValidator() {
        _mysqlClient = make_shared<MySQLClient>();
        // 初始化数据库连接
        _mysqlClient->connect("127.0.0.1", "root", "1234", "ning", 3306);
    }

    char* GenRandomString(int length)  {  
        int flag, i;  
        char* string;  
        srand((unsigned) time(NULL ));  
        if ((string = (char*) malloc(length)) == NULL )  {  
            printf("malloc failed!flag:14\n");  
            return NULL ;  
        }  
        for (i = 0; i < length+1; i++)  {  
            flag = rand() % 3;  
            switch (flag)  {  
            case 0:  string[i] = 'A' + rand() % 26;  break;  
            case 1:  string[i] = 'a' + rand() % 26;  break;  
            case 2:  string[i] = '0' + rand() % 10;  break;  
            default:  string[i] = 'x';  break;  
            }
        }
        string[length] = '\0';
        return string;  
    } 

    bool hasUser(const string& username) {
        string query = "SELECT COUNT(*) FROM users WHERE name = '" + username + "';";
        auto result = _mysqlClient->readOperationQuery(query);

        if (!result.empty() && result.size() > 1) {
            // 第一行是标题，第二行是结果
            if (result[1].size() > 0) {
                int count = stoi(result[1][0]);
                cout << "count = " << count << endl;
                return count > 0;
            }
        }
        return false;
    }

    string getSaltByUsername(const string& username) {
        string query = "SELECT salt FROM users WHERE name = '" + username + "';";
        auto result = _mysqlClient->readOperationQuery(query);

        if (!result.empty() && result.size() > 1) {
            // 第一行是标题，第二行是结果
            if (result[1].size() > 0) {
                return result[1][0]; // 返回 salt
            }
        }
        return ""; // 如果没有找到，返回空字符串
    }

    bool validate_password(const string& username, const string& client_encrypted_password) {
        string query = "SELECT salt, encrypted_password FROM users WHERE name = '" + username + "';";
        auto result = _mysqlClient->readOperationQuery(query);

        if (!result.empty() && result.size() > 1) {
            // 第一行是标题，第二行是结果
            if (result[1].size() >= 2) {
                string salt = result[1][0];
                string stored_encrypted_password = result[1][1];
                cout << "stored_encrypted_password = " << stored_encrypted_password << endl;
                cout << "client_encrypted_password = " << client_encrypted_password << endl;
                return client_encrypted_password == stored_encrypted_password;
            }
        }
        return false;
    }

    bool insertUser(const string& username, const string& salt, const string& ctypt) {
        // 构建插入语句
        string query = "INSERT INTO users (name, salt, encrypted_password) VALUES ('" + username + "', '" + salt + "', '" + ctypt + "');";
        cout << "Insert Query: " << query << endl;

        // 执行插入操作
        return _mysqlClient->writeOperationQuery(query);
    }
};
