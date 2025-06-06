#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include "FFmegKits.h"
#include "CryptString.h"
#include "CircularButtonWidget.h"
#include "HTTPRequest.h"
#include <QLabel>
#include <QWidget>
#include <QImage>
#include <memory>
#include <QListWidget>
#include <ClientHandler.h>


using std::unique_ptr;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWidget;
}
QT_END_NAMESPACE

enum PLAYER_STATE {
    PLAYER_IDLE = 0,
    PLAYER_PLAYING,
    PLAYER_PAUSE,
    PLAYER_STOP
};

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(const QString &username, QWidget *parent = nullptr);
    ~MainWidget();

    void paintEvent(QPaintEvent *event) override;
    void genRecordListRequestTest();
    void yuntaiRequest(const QString &value);
    void recv_msg();
signals:
    void sigGetOneFrame(QImage &image);

private slots:
    void getOneFrame(QImage image);

    void on_pauseButton_clicked();

    void on_playButton_clicked();

    void on_flipButton_clicked();
    void setUsername();
    void showImage(const QByteArray &data, const int &width, const int &height);

    //云台控制
    void Yuntai_Button_clicked(int idx);
    void center_Button_clicked();


private:
    QString _username;
    CircularButtonWidget *cirbutton;
    unique_ptr < FFmpegKits > _ffmpegKits;
    QImage _image;
    PLAYER_STATE _kPlayState;
    bool _hFlip;//水平翻转
    bool _vFlip;//垂直翻转
    HTTPRequest *request;
    ClientHandler *m_clientHandler;
    QListWidget *listwidget;

    QTcpSocket *m_tcpSocket;
};
#endif // MAINWIDGET_H
