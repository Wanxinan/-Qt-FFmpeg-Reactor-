#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

class HTTPRequest : public QObject
{
    Q_OBJECT

public:
    explicit HTTPRequest(QObject *parent = nullptr);
    ~HTTPRequest();

    void getData(const QString &url);
    void postData(const QString &url, const QByteArray &data);


private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager *m_manager;
    QNetworkReply *reply;
};

#endif // HTTPREQUEST_H
