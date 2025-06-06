#include "HTTPRequest.h"

HTTPRequest::HTTPRequest(QObject *parent)
    : QObject(parent),
      m_manager(new QNetworkAccessManager(this)),
      reply(nullptr)
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &HTTPRequest::onReplyFinished);
    qDebug() << "HTTPRequest()";
}

HTTPRequest::~HTTPRequest()
{
    if (reply) {
        reply->abort();
        reply->deleteLater();
    }
    delete m_manager;
}

void HTTPRequest::getData(const QString &url)
{
    QUrl q(url);
    QNetworkRequest request(q);
    reply = m_manager->get(request);
}

void HTTPRequest::postData(const QString &url, const QByteArray &data)
{
    QUrl q(url);
    QNetworkRequest request(q);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    reply = m_manager->post(request, data);
}

void HTTPRequest::onReplyFinished()
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QString response = QString::fromUtf8(data);
        qDebug() << "Response:" << response;
        // 这里可以添加处理响应数据的代码
    } else {
        qWarning() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
    reply = nullptr;
}


