#include "MainWidget.h"

#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QUrl>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

MainWidget::MainWidget(const QString &username, QWidget *parent)
    : QWidget(parent)
    , _ffmpegKits(new FFmpegKits)
    , _kPlayState(PLAYER_IDLE)
    , _hFlip(false)
    , _vFlip(false)
    , _username(username)
{
    resize(1100, 900);

    m_clientHandler = new ClientHandler(this);
    request = new HTTPRequest;
    m_tcpSocket = new QTcpSocket;
    // 创建水平布局（控制栏）
    // 创建水平布局
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(5, 5, 5, 5);  // 设置边距：左、上、右、下
    hLayout->setSpacing(5);  // 设置控件间距
    // 创建控件
    QPushButton *playbutton = new QPushButton("播放");
    QPushButton *pausebutton = new QPushButton("暂停");
    QPushButton *reversebutton = new QPushButton("获取传感器数据");
    cirbutton = new CircularButtonWidget;
    // 添加控件到水平布局
    hLayout->addWidget(reversebutton);
    hLayout->addWidget(playbutton);
    hLayout->addWidget(pausebutton);
    hLayout->addWidget(cirbutton);
    // 创建一个容器来放置水平布局
    QWidget *container = new QWidget;
    container->setLayout(hLayout);
    container->setStyleSheet("background-color: #f0f0f0;");
    // 创建垂直布局（主布局）
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    // 添加容器到垂直布局
    vLayout->addWidget(container);
    QHBoxLayout *listlayout = new QHBoxLayout;
    listwidget = new QListWidget;
    QString listStyle = "QListWidget {"
                            "background-color: white;"
                            "border: 1px solid #ddd;"
                            "border-radius: 4px;"
                            "padding: 5px;"
                            "font-size: 14px;"
                            "}"
                            "QListWidget::item {"
                            "padding: 8px;"
                            "border-bottom: 1px solid #eee;"
                            "}"
                            "QListWidget::item:hover {"
                            "background-color: #f5f5f5;"
                            "}";

    listwidget->setStyleSheet(listStyle);
    QWidget *fullwidget = new QWidget;
    listlayout->addWidget(listwidget, 1);
    listlayout->addWidget(fullwidget, 5);
    QWidget *container2 = new QWidget;
    container2->setLayout(listlayout);
    vLayout->addWidget(container2);
    // 添加一个可拉伸的空间
    vLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    connect(_ffmpegKits.get(), &FFmpegKits::sigGetOneFrame, this, &MainWidget::getOneFrame);
    connect(playbutton, &QPushButton::clicked, this, &MainWidget::on_playButton_clicked);
    connect(pausebutton, &QPushButton::clicked, this, &MainWidget::on_pauseButton_clicked);
    connect(reversebutton, &QPushButton::clicked, this, &MainWidget::on_flipButton_clicked);
    connect(cirbutton, &CircularButtonWidget::yuntai, this, &MainWidget::Yuntai_Button_clicked);
    //connect(cirbutton,&CircularButtonWidget::centerButtonClicked,this,&MainWidget::center_Button_clicked);
    connect(m_clientHandler, &ClientHandler::ImageMessage, this, &MainWidget::showImage);
    connect(this, &MainWidget::sigGetOneFrame, this, &MainWidget::getOneFrame);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &MainWidget::recv_msg);
    connect(m_tcpSocket, &QTcpSocket::disconnected, [ = ] {
        m_tcpSocket->deleteLater();
        m_tcpSocket->close();
    });
    m_tcpSocket->connectToHost("192.168.244.128", 8001);
}

MainWidget::~MainWidget()
{
}

void MainWidget::getOneFrame(QImage image)
{
    if(_kPlayState == PLAYER_PAUSE) {
        return;
    }
    _image = image;
    update(); //调用update将执行paintEvent函数
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int showWidth = this->width() - 100 - 150;
    int showHeight = this->height() - 150 - 150;  // 调整高度以适应顶部控制栏
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成白色
    if (_image.size().width() <= 0) {
        return;
    }
    //将图像按比例缩放
    QImage img = _image.scaled(QSize(showWidth, showHeight), Qt::KeepAspectRatio);
    img = img.mirrored(_hFlip, _vFlip);
    int x = this->width() - img.width();
    int y = this->height() - img.height();
    x /= 2;
    y /= 2;
    y += 50;  // 向下移动图像，避免被控制栏遮挡
    //qDebug() << "x:" << x << ", y:" << y << endl;
    painter.drawImage(QPoint(x + 100, y + 50), img); //画出图像
}

void MainWidget::genRecordListRequestTest()
{
    KVQuery kvs;
    kvs.add("start_time", "1673193600000");
    kvs.add("end_time", "1673279999999");
    kvs.add("channel", "0");
    QString tmp = QString::fromStdString(kvs.toString());
    qDebug() << "before crypt tmp:" << tmp << endl;
    // ------------------------------------------------
    CryptString cryptStr(tmp);
    QString token = cryptStr.generateMD5();
    qDebug() << "token:" << token << endl;//生成token
    kvs.add("token", token.toStdString());//加入参数
    kvs.erase("secret");//去掉secret
    //重新生成query
    tmp = QString::fromStdString(kvs.toString());
    qDebug() << "query:" << tmp << endl;
    // ------------------------------------------------
    QUrl url("http://192.168.106.122/xsw/api/reord/list");
    url.setQuery(tmp);
    qDebug() << "url:" << url.toString() << endl;
}

void MainWidget::yuntaiRequest(const QString &value)
{
    KVQuery kvs;
    kvs.add("channel", "0");
    kvs.add("value", value.toStdString());
    QString tmp = QString::fromStdString(kvs.toString());
    qDebug() << "before crypt tmp:" << tmp << endl;
    // ------------------------------------------------
    CryptString cryptStr(tmp);
    QString token = cryptStr.generateMD5();
    qDebug() << "token:" << token << endl;//生成token
    kvs.add("token", token.toStdString());//加入参数
    kvs.erase("secret");//去掉secret
    //重新生成query
    tmp = QString::fromStdString(kvs.toString());
    qDebug() << "query:" << tmp << endl;
    // ------------------------------------------------
    QUrl url("http://192.168.106.122/xsw/api/ptz/control");
    url.setQuery(tmp);
    qDebug() << "url:" << url.toString() << endl;
    QString body = "";
    //Qt直接与接口交互
    //request->getData(url.toString());
    //通过服务器转发
    m_clientHandler->cameraMove(_username, url.toString());
}



void MainWidget::on_pauseButton_clicked()
{
    yuntaiRequest("s");
    if (_kPlayState == PLAYER_PLAYING) {
        _kPlayState = PLAYER_PAUSE;
    } else if (_kPlayState == PLAYER_PAUSE) {
        _kPlayState = PLAYER_PLAYING;
    }
}

void MainWidget::on_playButton_clicked()
{
    if (_kPlayState == PLAYER_IDLE) { //第一次按下为启动，后续则为继续
        //视频流
        QString url = QString::fromUtf8("rtmp://192.168.106.122:1935/hlsram/live1");
        qDebug() << "url:" << url << endl;
        //Qt线程处理
        _ffmpegKits->startPlay(url);
        //转交给服务器获取然后返回客户端
        //->getVideoStream(_username,url);
        _kPlayState = PLAYER_PLAYING;
    } else {
        // 如果是播放/暂停状态切换，由暂停按钮处理
    }
}

void MainWidget::on_flipButton_clicked()
{
//    _hFlip = !_hFlip;
//    _vFlip = !_vFlip;  // 同时反转垂直方向
    qDebug() << "获取传感器数据";
    m_tcpSocket->write("HTTP/1.1");
}

void MainWidget::setUsername()
{
}

void MainWidget::showImage(const QByteArray &data, const int &width, const int &height)
{
    qDebug() << "data = " << data << "data_size = " << data.size();
//    QImage image;
//    if(image.loadFromData(data, "RGB32")){
//        qDebug() << "加载图片成功";
//    }else{
//        qDebug() << "加载图片失败";
//    }
//    qDebug() << "图片大小:" << data.size();
    QImage tmpImg((uchar *)(data.toStdString().c_str()),
                  width, height, QImage::Format_RGB32);
    QImage image = tmpImg.copy();
    emit sigGetOneFrame(image);
}

void MainWidget::Yuntai_Button_clicked(int idx)
{
    switch (idx) {
        case 6: {
            qDebug() << "上";
            yuntaiRequest("u");
            break;
        }
        case 7: {
            qDebug() << "右上";
            yuntaiRequest("1");
            break;
        }
        case 0: {
            qDebug() << "右";
            yuntaiRequest("r");
            break;
        }
        case 1: {
            qDebug() << "右下";
            yuntaiRequest("4");
            break;
        }
        case 2: {
            qDebug() << "下";
            yuntaiRequest("d");
            break;
        }
        case 3: {
            qDebug() << "左下";
            yuntaiRequest("3");
            break;
        }
        case 4: {
            qDebug() << "左";
            yuntaiRequest("l");
            break;
        }
        case 5: {
            qDebug() << "左上";
            yuntaiRequest("1");
            break;
        }
    }
}

void MainWidget::center_Button_clicked()
{
    qDebug() << "暂停";
    yuntaiRequest("s");
}

string removeQuotesAndBrackets(const string &s)
{
    string result;
    for (char c : s) {
        if (c != '"' && c != '[' && c != ']' && c != '\\') {
            result += c;
        }
    }
    return result;
}

// 分割字符串
vector < string > splitString(const string &s, char delimiter)
{
    vector < string > tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void MainWidget::recv_msg()
{
    QByteArray ba = m_tcpSocket->readAll();
    string text = QString::fromUtf8(ba).toStdString();
    size_t jsonStart = text.find("[");
    if (jsonStart == string::npos) {
        qDebug() << "JSON data not found in the response." ;
        return;
    }
    // 提取 JSON 数据部分
    string jsonData = text.substr(jsonStart);
    jsonData = removeQuotesAndBrackets(jsonData);
    // 分割 JSON 数据
    vector < string > sensorData = splitString(jsonData, ',');
    // 输出传感器数据
    for (const string &data : sensorData) {
        QString text = QString::fromStdString(data);
        qDebug() << text;
        listwidget->addItem(text);
    }
}
