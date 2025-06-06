#ifndef __WD_SOCKETIO_H__
#define __WD_SOCKETIO_H__
#include <iostream>
using std::string;
namespace wd
{

typedef struct tlv{
    int type;
    int length;
    char value[1024];
}TLV;

class SocketIO
{
public:
	explicit
	SocketIO(int fd);

	int readn(char * buff, int len);
	int readline(char * buff, int maxlen);
	int readTLV(string & s);
	int writen(const char * buff, int len);
	int writenn(const char * buff, int len);
	int recvPeek(char * buff, int len) const;
    int sendn(int sockfd, const char *buff, int len, int flags);

private:
	int _fd;
};

}//end of namespace wd

#endif 
