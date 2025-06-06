#include <iostream>
#include <string>
#include <cstring>
#include <shadow.h>
#include <unistd.h>

class PasswordValidator {
public:
    PasswordValidator() = default;

    void error_quit(const char* msg) const {
        perror(msg);
        exit(-2);
    }

    void get_setting(char* salt, const char* passwd) const {
        int i, j;
        for (i = 0, j = 0; passwd[i] && j != 4; ++i) {
            if (passwd[i] == '$') {
                ++j;
            }
        }
        strncpy(salt, passwd, i);
        salt[i] = '\0';
    }

    bool hasUser(const std::string& username){
        struct spwd* sp;

        // 读取 /etc/shadow 文件中对应的用户名的加密密文信息
        if ((sp = getspnam(username.c_str())) == nullptr) {
            return false;
        }
        return true;
    }

    bool validate_password(const std::string& username, const std::string& input_password) {
        struct spwd* sp;
        char setting[512] = {0};

        // 读取 /etc/shadow 文件中对应的用户名的加密密文信息
        if ((sp = getspnam(username.c_str())) == nullptr) {
            error_quit("用户名不存在");
        }

        //std::cout << "from shadow file, sp->sp_pwdp: " << sp->sp_pwdp << std::endl;

        get_setting(setting, sp->sp_pwdp);
        //std::cout << "setting: " << setting << std::endl;

        char* encoded = crypt(input_password.c_str(), setting);
        std::cout << "encrypted: " << encoded << std::endl;

        return strcmp(sp->sp_pwdp, encoded) == 0;
    }
};
