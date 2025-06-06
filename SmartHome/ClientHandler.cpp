#include "ClientHandler.h"
#include "CryptString.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <string.h>
#include <QNetworkProxy>

ClientHandler::ClientHandler(QObject *parent) : QObject(parent)
{
    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->setProxy(QNetworkProxy::NoProxy);
    connect(m_tcpSocket, &QTcpSocket::connected, this, []() {
        qDebug() << "Connection successful!";
    });
    connect(m_tcpSocket, QOverload < QAbstractSocket::SocketError >::of(&QTcpSocket::error),
    [this](QAbstractSocket::SocketError socketError) {
        qDebug() << "Socket error:" << socketError << m_tcpSocket->errorString();
    });
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ClientHandler::recv_msg);
    connect(m_tcpSocket, &QTcpSocket::disconnected, [ = ] {
        m_tcpSocket->deleteLater();
        m_tcpSocket->close();
    });
    //m_tcpSocket->connectToHost("192.168.200.128", 8000);
    m_tcpSocket->connectToHost("192.168.244.128", 8000);
    qDebug() << "ClientHandler()";
}

ClientHandler::~ClientHandler()
{
    if (m_tcpSocket) {
        m_tcpSocket->disconnectFromHost(); // 确保断开连接
        //m_tcpSocket->waitForDisconnected(); // 等待断开连接完成
        delete m_tcpSocket; // 删除 socket
        m_tcpSocket = nullptr; // 将指针置为 nullptr
    }
}


void ClientHandler::checkUsernameAvailable(const QString &username)
{
    TLV tlv;
    m_currentUsername = username;
    QString formattedUsername = QString("username=%1").arg(username);
    QString request = QString("type=1,client=qt,%1").arg(formattedUsername);
    QByteArray byteArray = request.toUtf8();
    char *sendData = byteArray.data();
    qDebug() << "Request:" << request;
    tlv.type = 2;
    tlv.length = strlen(sendData);
    qDebug() << "tlv.length:" << strlen(sendData);
    strcpy(tlv.value, sendData);
    QByteArray bytearray(reinterpret_cast < const char* > (&tlv), 8 + tlv.length);
    m_tcpSocket->write(bytearray);
}

void ClientHandler::checkUsernameAvailable2(const QString &username)
{
    TLV tlv;
    m_currentUsername = username;
    QString formattedUsername = QString("username=%1").arg(username);
    QString request = QString("type=7,client=qt,%1").arg(formattedUsername);
    QByteArray byteArray = request.toUtf8();
    char *sendData = byteArray.data();
    qDebug() << "Request:" << request;
    tlv.type = 2;
    tlv.length = strlen(sendData);
    qDebug() << "tlv.length:" << strlen(sendData);
    strcpy(tlv.value, sendData);
    QByteArray bytearray(reinterpret_cast < const char* > (&tlv), 8 + tlv.length);
    m_tcpSocket->write(bytearray);
}

void ClientHandler::checkPasswordAvailable(const QString &username, const QString &password, const QString &salt)
{
    TLV tlv;
    CryptString cryptstring(password + salt);
    QString crypt = cryptstring.generateMD5();
    QString formattedUsername = QString("username=%1").arg(username);
    QString formattedPassword = QString("crypt=%1").arg(crypt);
    QString request = QString("type=4,client=qt,%1,%2").arg(formattedUsername).arg(formattedPassword);
    QByteArray byteArray = request.toUtf8();
    char *sendData = byteArray.data();
    qDebug() << "Request:" << request;
    tlv.type = 2;
    tlv.length = strlen(sendData);
    qDebug() << "tlv.length:" << tlv.length;
    strcpy(tlv.value, sendData);
    QByteArray bytearray(reinterpret_cast < const char* > (&tlv), 8 + tlv.length);
    m_tcpSocket->write(bytearray);
}

void ClientHandler::sendPassword(const QString &username, const QString &salt, const QString &ctypt)
{
    TLV tlv;
    QString formattedUsername = QString("username=%1").arg(username);
    QString formattedSalt = QString("salt=%1").arg(salt);
    QString formattedCtypt = QString("ctypt=%1").arg(ctypt);
    QString request = QString("type=10,client=qt,%1,%2,%3")
                      .arg(formattedUsername)
                      .arg(formattedSalt)
                      .arg(formattedCtypt);
    QByteArray byteArray = request.toUtf8();
    char *sendData = byteArray.data();
    qDebug() << "Request:" << request;
    tlv.type = 2;
    tlv.length = strlen(sendData);
    qDebug() << "tlv.length:" << tlv.length;
    strcpy(tlv.value, sendData);
    QByteArray bytearray(reinterpret_cast < const char* > (&tlv), 8 + tlv.length);
    m_tcpSocket->write(bytearray);
}

void ClientHandler::getVideoStream(const QString &username, const QString &url)
{
    TLV tlv;
    QString formattedUsername = QString("username=%1").arg(username);
    QString formattedUserurl = QString("url=%1").arg(url);
    QString request = QString("type=100,client=qt,%1,%2")
                      .arg(formattedUsername)
                      .arg(formattedUserurl);
    QByteArray byteArray = request.toUtf8();
    char *sendData = byteArray.data();
    qDebug() << "Request:" << request;
    tlv.type = 2;
    tlv.length = strlen(sendData);
    qDebug() << "tlv.length:" << tlv.length;
    strcpy(tlv.value, sendData);
    QByteArray bytearray(reinterpret_cast < const char* > (&tlv), 8 + tlv.length);
    m_tcpSocket->write(bytearray);
}

void ClientHandler::cameraMove(const QString &username, const QString &url)
{
    TLV tlv;
    QString formattedUsername = QString("username=%1").arg(username);
    QString formattedUrl = QString("url=%1").arg(url);
    QString request = QString("type=103,client=qt,%1,%2")
                      .arg(formattedUsername)
                      .arg(formattedUrl);
    QByteArray byteArray = request.toUtf8();
    char *sendData = byteArray.data();
    qDebug() << "Request:" << request;
    tlv.type = 2;
    tlv.length = strlen(sendData);
    qDebug() << "tlv.length:" << tlv.length;
    strcpy(tlv.value, sendData);
    QByteArray bytearray(reinterpret_cast < const char* > (&tlv), 8 + tlv.length);
    m_tcpSocket->write(bytearray);
}

void ClientHandler::analysisByte(QByteArray & ba)
{
    QString response = QString::fromUtf8(ba);
    // 检查响应是否包含有效的键值对数据
    if (!response.contains("=")) {
        qDebug() << "Key-value pairs not found in the response.";
        return;
    }
    // 分割键值对
    QStringList keyValuePairs = response.split(',');
    // 解析每个键值对
    for (const QString &pair : keyValuePairs) {
        QStringList keyValue = pair.split('=');
        if (keyValue.size() == 2) {
            QString key = keyValue[0].trimmed();
            QString value = keyValue[1].trimmed();
            dataMap.insert(key, value);
        }
    }
    // 输出解析后的键值对
    for (auto it = dataMap.constBegin(); it != dataMap.constEnd(); ++it) {
        //qDebug() << it.key() << ":" << it.value();
    }
}

//void ClientHandler::analysisVideoStream(const QString & response)
//{
//    QStringList parts = response.split(',');
//    QString type;
//    QByteArray base64Data;
//    int width = 0, height = 0;
//    for (const QString &part : parts) {
//        if (part.startsWith("type=")) {
//            type = part.split('=').last();
//        } else if (part.startsWith("data=")) {
//            base64Data = QByteArray::fromBase64(part.split('=').last().toLatin1());
//        } else if (part.startsWith("width=")) {
//            width = part.split('=').last().toInt();
//        } else if (part.startsWith("height=")) {
//            height = part.split('=').last().toInt();
//        }
//    }
//    emit ImageMessage(base64Data, width, height);
//}

//服务器回消息
void ClientHandler::recv_msg()
{
    qDebug() << "收到服务器消息" << endl;
    QByteArray preview = m_tcpSocket->peek(100);
    analysisByte(preview);
    int type;
    qDebug() << dataMap["type"] ;
    if(!isVideoStream && atoi(dataMap["type"].toStdString().c_str()) != TASK_GET_VIDEO_STREAM_OK) {
        QByteArray ba = m_tcpSocket->readAll();
        //解析数据并存在map中
        analysisByte(ba);
        type = atoi(dataMap["type"].toStdString().c_str());
        //qDebug() << "非视频流";
    } else {
        qDebug() << "视频流";
        if(!isVideoStream){
            width = atoi(dataMap["width"].toStdString().c_str());
            height = atoi(dataMap["height"].toStdString().c_str());
        }
        isVideoStream = true;
        // 1. 读取新数据并追加到缓冲区
        QByteArray newData = m_tcpSocket->readAll();
        buffer.append(newData);
        qDebug() << "buffer_Size = " << buffer.size();
        if(buffer.size() < 1638450) {
            return;
        }
        isVideoStream = false;
        type = TASK_GET_VIDEO_STREAM_OK;
    }
    switch (type) {
        case TASK_LOGIN_SECTION1_RESP_OK: {
            qDebug() << "收到服务器的第一次登录确认";
            QString salt = dataMap["salt"];
            emit usernameCheckedLogin1(true, m_currentUsername, salt);
            break;
        }
        case TASK_LOGIN_SECTION1_RESP_ERROR: {
            qDebug() << "收到服务器的第一次错误登录确认";
            emit usernameCheckedLogin1(false, m_currentUsername, "");
            break;
        }
        case TASK_LOGIN_SECTION2_RESP_OK: {
            qDebug() << "收到服务器的第二次登录确认";
            emit usernameCheckedLogin2(true, m_currentUsername);
            break;
        }
        case TASK_LOGIN_SECTION2_RESP_ERROR: {
            qDebug() << "收到服务器的第二次错误登录确认";
            emit usernameCheckedLogin2(false, m_currentUsername);
            break;
        }
        case TASK_REGISTER_SECTION1_RESP_OK: {
            qDebug() << "收到服务器的第一次注册确认";
            QString salt = dataMap["salt"];
            qDebug() << "服务器生成盐值:" << salt;
            emit usernameCheckedRegister1(true, m_currentUsername, salt);
            break;
        }
        case TASK_REGISTER_SECTION1_RESP_ERROR: {
            qDebug() << "收到服务器的第一次错误注册确认";
            emit usernameCheckedRegister1(false, m_currentUsername, nullptr);
            break;
        }
        case TASK_REGISTER_SECTION2_RESP_OK: {
            qDebug() << "收到服务器的第二次注册确认";
            emit usernameCheckedRegister2(true);
            break;
        }
        case TASK_REGISTER_SECTION2_RESP_ERROR: {
            qDebug() << "收到服务器的第二次错误确认";
            emit usernameCheckedRegister2(false);
            break;
        }
        case TASK_GET_VIDEO_STREAM_OK: {
            qDebug() << "收到服务器回传的视频流确认信息" << endl;
            buffer.remove(0, 50);
            emit ImageMessage(buffer, width, height);
            break;
        }
        case TASK_GET_VIDEO_STREAM_ERROR: {
            qDebug() << "收到服务器回传的视频流失败确认信息";
            break;
        }
        case TASK_CAMERA_MOVE_OK: {
            qDebug() << "收到服务器回传的摄像头确认信息";
            break;
        }
        case TASK_CAMERA_MOVE_ERROR: {
            qDebug() << "收到服务器回传的摄像头失败确认信息";
            break;
        }
    }
    //处理完消息后清空map
    dataMap.clear();
}


