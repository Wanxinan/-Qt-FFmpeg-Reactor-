#include "my_header.h"

typedef struct tlv{
    int type;
    int length;
    char value[1024];
}TLV;
enum TaskType {
    TASK_LOGIN_SECTION1 = 1,
    TASK_LOGIN_SECTION1_RESP_OK,
    TASK_LOGIN_SECTION1_RESP_ERROR,
    TASK_LOGIN_SECTION2,
    TASK_LOGIN_SECTION2_RESP_OK,
    TASK_LOGIN_SECTION2_RESP_ERROR,
};
int main()
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(-1, cfd, "socket");

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8000);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret = connect(cfd,  (const struct sockaddr*)&serveraddr, sizeof(serveraddr));
    ERROR_CHECK(ret, -1, "connect");

    printf("conn has connected.\n");

    fd_set set;

    while(1){
        //在用户态每一次轮询之前都要添加监听
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(cfd, &set);

        //set既是传入参数，也是传出参数
        select(cfd +1, &set, NULL, NULL, NULL);
        //当select函数返回时，set已经被修改了
        //如果是有100个文件描述符就绪，我们是否知道哪些文件描述符发生了事件
        //因此还需要在用户态继续轮询
        if(FD_ISSET(cfd, &set)){
            char buf[60] = {0};
            ssize_t ret = recv(cfd, buf, sizeof(buf), 0);
            if(ret == 0){
                // 可读, 但是读到数据个数0个
                // 说明对方断开链接
                printf("对方断开链接 \n");
                break;
            }
            int type = atoi(buf);
            switch(type){
            case TASK_LOGIN_SECTION1_RESP_OK:{
                                                 printf("login1OK\n");
                                                 break;
                                             }
            case TASK_LOGIN_SECTION1_RESP_ERROR:{

                                                    printf("login1ERROR\n");
                                                    break;
                                                }
            case TASK_LOGIN_SECTION2_RESP_OK:{
                                                 printf("login2OK\n");
                                                 break;
                                             }
            case TASK_LOGIN_SECTION2_RESP_ERROR:{
                                                    printf("login2ERROR\n");
                                                    break;
                                                }
            }
        }
        if(FD_ISSET(STDIN_FILENO, &set)){
            char buf[60] = {0};
            memset(buf,0,sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            printf("%s\n",buf);
            if(strcmp(buf,"1\n") == 0){
                TLV tlv;
                tlv.type = 2;
                strcpy(tlv.value,"type=1,client=ubuntu,username=ning");
                tlv.length = strlen(tlv.value);
                printf("%s\n", tlv.value);
                int ret = send(cfd, &tlv, 8 + tlv.length, 0);
                printf("send %d bytes.\n", ret);
            }else if(strcmp(buf,"2\n") == 0){
                TLV tlv;
                tlv.type = 2;
                strcpy(tlv.value,"type=4,client=ubuntu,username=ning,password=123");
                tlv.length = strlen(tlv.value);
                printf("%s\n", tlv.value);
                int ret = send(cfd, &tlv, 8 + tlv.length, 0);
                printf("send %d bytes.\n", ret);
            }
        }
    }

    close(cfd);

    return 0;
}
