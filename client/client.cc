#include <head.h>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::cin;

struct TLV
{
    int type;
    int length;
    char data[1000];
};

int main()
{
    //1. 客户端创建套接字
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(cfd, -1, "socket");
    //2. 连接服务器
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8000);
    serveraddr.sin_addr.s_addr = inet_addr("192.168.30.129");
    int ret = connect(cfd, 
                      (const struct sockaddr *)&serveraddr,
                      sizeof(serveraddr));
    ERROR_CHECK(ret, -1, "connect");
    cout << "conn has connected.\n";

    while(1)
    {
        cout << ">> input:";
        string line;
        cin >> line;
        int id = 1;
        int len = line.length();
        //测试: 发用户名
        send(cfd, &id, 4, 0);
        send(cfd, &len, 4, 0);
        send(cfd, line.c_str(), line.size(), 0);
        cout << "send " << 8 + len << " bytes." << endl;

        TLV tlv = {0};
        recv(cfd, &tlv.type, 4, 0);
        recv(cfd, &tlv.length, 4, 0);
        recv(cfd, tlv.data, tlv.length, 0);
        cout << ">> recv from server:\n" << tlv.data << ", len:" << tlv.length <<endl;
    }
    close(cfd);
    return 0;
}
