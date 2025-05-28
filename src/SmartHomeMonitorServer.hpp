#ifndef __SmartHomeMonitorServer_H__
#define __SmartHomeMonitorServer_H__

#include "Threadpool.hpp"
#include "TcpServer.hpp"

class SmartHomeMonitorServer
{
public:
    SmartHomeMonitorServer(int threadNum, int taskSize, unsigned short port, const string & ip = "0.0.0.0");

    void start();
    void stop();
private:
    void onConnection(wd::TcpConnectionPtr conn);
    void onMessage(wd::TcpConnectionPtr conn);
    void onClose(wd::TcpConnectionPtr conn);

private:
    wd::Threadpool _threadpool;
    wd::TcpServer _server;
};


#endif

