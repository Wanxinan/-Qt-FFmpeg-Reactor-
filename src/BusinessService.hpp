#ifndef __BusinessService_H__
#define __BusinessService_H__

#include "Message.hpp"
#include "TcpConnection.hpp"

class UserLoginSection1
{
public:
    UserLoginSection1(wd::TcpConnectionPtr conn, const wd::Packet & p)
    : _conn(conn)
    , _packet(p)
    {}

    //业务逻辑的处理
    void process();

private:
    void getSetting(string & s, const char * passwd);

private:
    wd::TcpConnectionPtr _conn;
    wd::Packet _packet;
};

class UserLoginSection2
{
public:
    UserLoginSection2(wd::TcpConnectionPtr conn, const wd::Packet & p)
    : _conn(conn)
    , _packet(p)
    {}

    //业务逻辑的处理
    void process();


private:
    wd::TcpConnectionPtr _conn;
    wd::Packet _packet;
};

#endif

