#include "SmartHomeMonitorServer.hpp"
#include "BusinessService.hpp"
#include "Message.hpp"
#include "Mylogger.hpp"
#include <iostream>

using std::cout;
using std::endl;


SmartHomeMonitorServer::SmartHomeMonitorServer(int threadNum, 
        int taskSize, 
        unsigned short port, 
        const string & ip)
: _threadpool(threadNum, taskSize)
, _server(port, ip)
{
    using namespace std::placeholders;
    _server.setAllCallbacks(
        std::bind(&SmartHomeMonitorServer::onConnection, this, _1),
        std::bind(&SmartHomeMonitorServer::onMessage, this, _1),
        std::bind(&SmartHomeMonitorServer::onClose, this, _1));
}

void SmartHomeMonitorServer::start()
{
    _threadpool.start();
    _server.start();
}


void SmartHomeMonitorServer::stop()
{
    _server.stop();
    _threadpool.stop();
}

void SmartHomeMonitorServer::onConnection(wd::TcpConnectionPtr conn)
{
    LogInfo("tcp %s has connected.\n", conn->toString().c_str());
}

void SmartHomeMonitorServer::onMessage(wd::TcpConnectionPtr conn)
{
    cout << "onMessage..." << endl;
    wd::Packet packet;
    int ret = conn->readPacket(packet);
    cout << "read:" << ret << "bytes.\n";
    cout << "packet.type: " << packet.type << endl
         << "packet.length:" << packet.length << endl
         << "pakcet.msg:" << packet.msg << endl;

    //handleTask((wd::TaskType)packet.type);
    switch(packet.type)
    {
    case wd::TASK_TYPE_LOGIN_SECTION1:
        {
            UserLoginSection1 userLogin1(conn, packet);
            _threadpool.addTask(std::bind(&UserLoginSection1::process, userLogin1));
        }
        break;
    case wd::TASK_TYPE_LOGIN_SECTION2:
        {}
        break;
    }
}

void SmartHomeMonitorServer::onClose(wd::TcpConnectionPtr conn)
{
    LogInfo("tcp %s has closed.\n", conn->toString().c_str());
}


