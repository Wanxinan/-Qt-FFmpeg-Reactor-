#include "my_header.h"
typedef struct INFO{
    char key;
    char value[10];
}info;

int main() {


    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    char *port = "8001";
    char *ip = "0.0.0.0";
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port));
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    int reuse = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    bind(socket_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    listen(socket_fd, 10);

    while(1){
        int net_fd = accept(socket_fd, NULL, NULL);
        printf("net_fd = %d\n",net_fd);
        //STM32写文件或者手机获取文件内容
        int fd = open("info.txt",O_RDWR|O_APPEND);
                
        char buf[1024] = {0};
        recv(net_fd, buf, sizeof(buf), 0);
        if(strlen(buf) == 0){
            close(fd);
            close(net_fd);
            continue;
        }
        //printf("%s\n", buf);
        char *buf2 = strstr(buf,"/"); 
        char flag = buf2[1];
        //printf("%c\n",flag);
        if(flag == '1'){
            printf("手机\n");
            //char *str1 = "HTTP/1.1 200 OK\r\n\r\n[\"test1\",\"test2\",\"test3\"]";
            char *str1 = "HTTP/1.1 200 OK\r\n\r\n[";
            char buffer[2048] = {0};
            int offset = sprintf(buffer,"%s",str1);
            char sensor[1024] = {0};
            while(1){
                info send_info;
                int num = read(fd,&send_info,sizeof(send_info));
                if(num == -1){
                    printf("出错\n");
                    break;
                }
                if(num == 0){
                    buffer[offset - 1] = ']';
                    break;
                }
                switch(send_info.key){
                case '1':
                    sprintf(sensor,"%s","光敏电阻");
                    break;
                case '2':
                    sprintf(sensor,"%s","空气质量");
                    break;
                case '3':
                    sprintf(sensor,"%s","红外传感器");
                    break;
                case '4':
                    sprintf(sensor,"%s","热敏电阻");
                    break;
                }
                printf("key=%s,value=%s\n",sensor,send_info.value);
                int ret_offset = sprintf(buffer+offset,"%c%s%c%s%c%c",'\"',sensor,':',send_info.value,'\"',',');
                offset += ret_offset;
            }
            send(net_fd, buffer, strlen(buffer), 0);
            printf("%s\n", buffer);
        }
        else if(flag == '2'){
            close(fd);
            int fd2 = open("info.txt",O_RDWR | O_TRUNC);
            printf("文件已清空\n");
            close(fd2);
        }
        else if(flag == '3'){
            printf("STM32向服务器传输数据\n");
            for(int i = 0;i < 12;i++){
                if(i != 0){
                    memset(buf,0,sizeof(buf));
                    recv(net_fd, buf, sizeof(buf), 0);
                }
                info new_info;
                memset(new_info.value,0,sizeof(new_info.value));
                char* key = strstr(buf,"=");
                key++;
                new_info.key = key[0];
                char *value = strstr(key,"=");
                value++;
                int size = 0;
                while(value[size] != '#'){
                    new_info.value[size] = value[size];
                    size++;
                }
                printf("key = %c,value = %s\n",new_info.key,new_info.value);
                write(fd,&new_info,sizeof(new_info));
                send(net_fd, "hello", 5, 0);
            }
        }
        printf("服务器收到的信息:%s\n",buf);
        close(fd);
        close(net_fd);
    }



    //while(1);
    close(socket_fd);
    return 0;

}
