#ifndef FFMPEGKITS_H
#define FFMPEGKITS_H

#include <QThread>
#include <QImage>
//继承自QThread
class FFmpegKits : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegKits(QObject *parent = nullptr);

    void startPlay(QString url);

protected:
    void run() override;

signals:
    void sigGetOneFrame(QImage image);

private:
    QString _url;
};

#endif // FFMPEGKITS_H
