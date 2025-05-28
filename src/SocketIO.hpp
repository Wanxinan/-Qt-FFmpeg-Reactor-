#ifndef __WD_SOCKETIO_H__
#define __WD_SOCKETIO_H__
#include "Message.hpp"

namespace wd
{


class SocketIO
{
public:
	explicit
	SocketIO(int fd);

	int readn(char * buff, int len);
	int readline(char * buff, int maxlen);
    int readPacket(Packet & packet);
	int writen(const char * buff, int len);
	int recvPeek(char * buff, int len) const;

private:
	int _fd;
};

}//end of namespace wd

#endif 
