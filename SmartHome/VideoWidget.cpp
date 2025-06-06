#include "VideoWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTextBrowser>
#include <QListWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QSlider>
#include <QSpinBox>
#include <QTime>
#include <QTimer>
#include <QDebug>

VideoWidget::VideoWidget(QWidget *parent) : QLabel(parent) {
    setFrameShape(QFrame::Box);
    setAlignment(Qt::AlignCenter);
    setText("未连接视频");
}

void VideoWidget::setVideoSource(const QString &source) {
    // 这里应该是实际设置视频源的代码
    // 实际应用中会使用FFmpeg解码并显示视频
    setText(QString("视频源: %1").arg(source));
}

VideoMonitorWidget::VideoMonitorWidget(QWidget *parent) : QWidget(parent) {
    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左侧面板
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    // 窗口信息区域
    QGroupBox *windowInfoGroup = new QGroupBox("窗口信息", leftPanel);
    QVBoxLayout *windowInfoLayout = new QVBoxLayout(windowInfoGroup);

    // 窗口信息浏览器
    QTextBrowser *windowInfoBrowser = new QTextBrowser(windowInfoGroup);
    windowInfoBrowser->setFixedHeight(150);
    updateWindowInfo(windowInfoBrowser);

    windowInfoLayout->addWidget(windowInfoBrowser);

    // 图文警情区域
    QGroupBox *alertGroup = new QGroupBox("图文警情", leftPanel);
    QVBoxLayout *alertLayout = new QVBoxLayout(alertGroup);

    // 警情列表
    QListWidget *alertList = new QListWidget(alertGroup);
    alertList->setFixedHeight(200);
    updateAlertList(alertList);

    alertLayout->addWidget(alertList);

    // 视频轮询按钮
    QPushButton *videoPollBtn = new QPushButton("视频轮询", leftPanel);
    videoPollBtn->setFixedHeight(30);
    connect(videoPollBtn, &QPushButton::clicked, this, &VideoMonitorWidget::startVideoPoll);

    leftLayout->addWidget(windowInfoGroup);
    leftLayout->addWidget(alertGroup);
    leftLayout->addWidget(videoPollBtn);

    // 中间视频监控区域
    QWidget *videoArea = new QWidget(this);
    QGridLayout *videoGrid = new QGridLayout(videoArea);

    // 创建4个视频通道
    for (int i = 0; i < 4; ++i) {
        VideoWidget *videoLabel = new VideoWidget(videoArea);
        videoLabels.append(videoLabel);
        videoGrid->addWidget(videoLabel, i/2, i%2);
    }

    // 底部时间显示
    QLabel *timeLabel = new QLabel(videoArea);
    timeLabel->setAlignment(Qt::AlignCenter);

    // 模拟时间更新
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        timeLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
    });
    timer->start(1000);

    videoGrid->addWidget(timeLabel, 2, 0, 1, 2); // 跨两行两列

    // 右侧面板
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // 设备列表
    QGroupBox *deviceGroup = new QGroupBox("设备列表", rightPanel);
    QVBoxLayout *deviceLayout = new QVBoxLayout(deviceGroup);

    QListWidget *deviceList = new QListWidget(deviceGroup);
    QStringList devices = {"默认分组", "网络视频", "网络视频1", "主码流", "子码流",
                          "网络视频2", "网络视频3", "网络视频4"};
    deviceList->addItems(devices);

    deviceLayout->addWidget(deviceList);

    // 云台控制
    QGroupBox *ptzGroup = new QGroupBox("云台控制", rightPanel);
    QVBoxLayout *ptzLayout = new QVBoxLayout(ptzGroup);

    // 方向控制按钮
    QWidget *directionControl = new QWidget(ptzGroup);
    QGridLayout *directionLayout = new QGridLayout(directionControl);

    QPushButton *upBtn = new QPushButton("↑", directionControl);
    QPushButton *leftBtn = new QPushButton("←", directionControl);
    QPushButton *rightBtn = new QPushButton("→", directionControl);
    QPushButton *downBtn = new QPushButton("↓", directionControl);

    directionLayout->addWidget(upBtn, 0, 1);
    directionLayout->addWidget(leftBtn, 1, 0);
    directionLayout->addWidget(rightBtn, 1, 2);
    directionLayout->addWidget(downBtn, 2, 1);

    // 连接方向控制按钮
    connect(upBtn, &QPushButton::clicked, [=]() { ptzControl("up"); });
    connect(downBtn, &QPushButton::clicked, [=]() { ptzControl("down"); });
    connect(leftBtn, &QPushButton::clicked, [=]() { ptzControl("left"); });
    connect(rightBtn, &QPushButton::clicked, [=]() { ptzControl("right"); });

    // 变倍、聚焦、光圈、步长控制
    QHBoxLayout *controlLayout = new QHBoxLayout();

    QLabel *zoomLabel = new QLabel("变倍", ptzGroup);
    QSlider *zoomSlider = new QSlider(Qt::Horizontal, ptzGroup);
    zoomSlider->setRange(0, 100);
    connect(zoomSlider, &QSlider::valueChanged, [=](int value) { ptzControl("zoom", value); });

    QLabel *focusLabel = new QLabel("聚焦", ptzGroup);
    QSlider *focusSlider = new QSlider(Qt::Horizontal, ptzGroup);
    focusSlider->setRange(0, 100);
    connect(focusSlider, &QSlider::valueChanged, [=](int value) { ptzControl("focus", value); });

    QLabel *irisLabel = new QLabel("光圈", ptzGroup);
    QSlider *irisSlider = new QSlider(Qt::Horizontal, ptzGroup);
    irisSlider->setRange(0, 100);
    connect(irisSlider, &QSlider::valueChanged, [=](int value) { ptzControl("iris", value); });

    QLabel *stepLabel = new QLabel("步长", ptzGroup);
    QSpinBox *stepSpin = new QSpinBox(ptzGroup);
    stepSpin->setRange(1, 10);
    stepSpin->setValue(5);
    connect(stepSpin, QOverload<int>::of(&QSpinBox::valueChanged),
           [=](int value) { ptzControl("step", value); });

    controlLayout->addWidget(zoomLabel);
    controlLayout->addWidget(zoomSlider);
    controlLayout->addWidget(focusLabel);
    controlLayout->addWidget(focusSlider);
    controlLayout->addWidget(irisLabel);
    controlLayout->addWidget(irisSlider);
    controlLayout->addWidget(stepLabel);
    controlLayout->addWidget(stepSpin);

    ptzLayout->addWidget(directionControl);
    ptzLayout->addLayout(controlLayout);

    rightLayout->addWidget(deviceGroup);
    rightLayout->addWidget(ptzGroup);

    // 添加到主布局
    mainLayout->addWidget(leftPanel, 1);
    mainLayout->addWidget(videoArea, 3);
    mainLayout->addWidget(rightPanel, 1);

    // 设置样式
    setStyleSheet("QGroupBox { border: 1px solid gray; border-radius: 5px; margin-top: 10px; } "
                 "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px; } "
                 "QLabel { border: 1px solid gray; } "
                 "QPushButton { min-width: 40px; min-height: 40px; }");
}

void VideoMonitorWidget::updateWindowInfo(QTextBrowser *browser) {
    QStringList infoItems = {
        "00:25:23 通道4 关闭视频",
        "00:25:23 按下按钮 banner_btnClose",
        "00:25:19 按下按钮 banner_btnSound",
        "00:25:15 按下按钮 banner_btnMuted",
        "00:25:06 通道04 打开 主码流"
    };

    browser->clear();
    for (const QString &item : infoItems) {
        browser->append(item);
    }
}

void VideoMonitorWidget::updateAlertList(QListWidget *listWidget) {
    QStringList alertItems = {
        "00:24:44 识别到人脸 待处理!",
        "00:24:43 有可疑车辆 已处理!",
        "00:24:42 检测到人脸 已处理!",
        "00:24:41 识别到人脸 待处理!",
        "00:24:40 画面有变动 待处理!",
        "00:24:39 识别到人脸 已处理!",
        "00:24:38 检测到人脸 待处理!",
        "00:24:37 有可疑车辆 已处理!"
    };

    listWidget->clear();
    for (const QString &item : alertItems) {
        QListWidgetItem *alertItem = new QListWidgetItem(item);
        listWidget->addItem(alertItem);
    }
}

void VideoMonitorWidget::startVideoPoll() {
    // 模拟视频轮询
    qDebug() << "开始视频轮询";

    // 实际应用中，这里会切换不同的视频源
    for (int i = 0; i < videoLabels.size(); ++i) {
        videoLabels[i]->setVideoSource(QString("摄像头%1").arg(i+1));
    }
}

void VideoMonitorWidget::ptzControl(const QString &command, int value) {
    // 模拟云台控制
    QString message;

    if (command == "up" || command == "down" || command == "left" || command == "right") {
        message = QString("云台方向控制: %1").arg(command);
    } else if (command == "zoom" || command == "focus" || command == "iris") {
        message = QString("云台%1调整: %2").arg(command).arg(value);
    } else if (command == "step") {
        message = QString("云台步长设置: %1").arg(value);
    }

    qDebug() << message;

    // 实际应用中，这里会发送控制命令到设备
}
