// clienthandler.h
#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

enum TaskType {
    TASK_LOGIN_SECTION1 = 1,
    TASK_LOGIN_SECTION1_RESP_OK,
    TASK_LOGIN_SECTION1_RESP_ERROR,
    TASK_LOGIN_SECTION2,
    TASK_LOGIN_SECTION2_RESP_OK,
    TASK_LOGIN_SECTION2_RESP_ERROR,
    TASK_REGISTER_SECTION1,
    TASK_REGISTER_SECTION1_RESP_OK,
    TASK_REGISTER_SECTION1_RESP_ERROR,
    TASK_REGISTER_SECTION2,
    TASK_REGISTER_SECTION2_RESP_OK,
    TASK_REGISTER_SECTION2_RESP_ERROR,
};

enum CameraControl {
    TASK_GET_VIDEO_STREAM = 100,
    TASK_GET_VIDEO_STREAM_OK,
    TASK_GET_VIDEO_STREAM_ERROR,
    TASK_CAMERA_MOVE,
    TASK_CAMERA_MOVE_OK,
    TASK_CAMERA_MOVE_ERROR,
};

typedef struct tlv {
    int type;
    int length;
    char value[1024];
} TLV;

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject *parent = nullptr);
    ~ClientHandler();

    void checkUsernameAvailable(const QString &username);
    void checkUsernameAvailable2(const QString &username);
    void checkPasswordAvailable(const QString &username, const QString &password, const QString &salt);
    void sendPassword(const QString &username, const QString &salt, const QString &ctypt);
    void getVideoStream(const QString &username, const QString &url);
    void cameraMove(const QString &username, const QString &url);

signals:
    void usernameCheckedLogin1(bool available, const QString &username, const QString &salt);
    void usernameCheckedLogin2(bool available, const QString &username);
    void usernameCheckedRegister1(bool available, const QString &username, const QString &salt);
    void usernameCheckedRegister2(bool available);
    void errorOccurred(const QString &error);
    void ImageMessage(const QByteArray &data, const int &width, const int &height);

private slots:
    //void onReplyFinished(QNetworkReply *reply);
    void recv_msg();


private:
    QTcpSocket *m_tcpSocket;
    QString m_currentUsername;
    QMap < QString, QString > dataMap;
    void analysisByte(QByteArray &);
    QByteArray buffer;
    QByteArray chunk;
    QString receivedString;
    bool isVideoStream = false;
    int stream_len;
    int width;
    int height;
    //void analysisVideoStream(const QString &);
};

#endif // CLIENTHANDLER_H
