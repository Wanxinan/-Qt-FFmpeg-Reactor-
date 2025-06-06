#include "CircularButtonWidget.h"
#include <QDebug>

CircularButtonWidget::CircularButtonWidget(QWidget *parent) : QWidget(parent) {
    setupUI();
}

CircularButtonWidget::~CircularButtonWidget() {

}

void CircularButtonWidget::buttonClicked(int idx)
{
    emit yuntai(idx);
}

void CircularButtonWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    //label = new QLabel("云台控制", this);
    //label->setAlignment(Qt::AlignCenter);
    //mainLayout->addWidget(label);

    // 创建一个中心部件来放置圆形按钮
    QWidget *centerWidget = new QWidget(this);
    centerWidget->setFixedSize(200, 200);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);
    centerLayout->addStretch(); // 添加弹性空间

    // 创建中间的按钮
    //createCentralButton(centerLayout);

    // 创建圆形按钮
    createCircularButtons();

    centerLayout->addStretch(); // 添加弹性空间
    mainLayout->addWidget(centerWidget, 0, Qt::AlignCenter);
}

//void CircularButtonWidget::createCentralButton(QVBoxLayout *centerLayout) {
//    centerButton = new QPushButton(this);
//    centerButton->setIcon(QIcon(":/image/sun.png"));
//    centerButton->setIconSize(QSize(50, 50));
//    centerButton->setFixedSize(50, 50);
//    //centerButton->setText("pause");
//    centerButton->setStyleSheet("QPushButton { border: none; }");
//    centerLayout->addWidget(centerButton, 0, Qt::AlignCenter);
//}

void CircularButtonWidget::createCircularButtons() {
    buttonsWidget = new QWidget(this);
    buttonsWidget->setFixedSize(200, 200);
    buttonsWidget->setStyleSheet("background-color: #4C4C4C; border-radius: 100;");
    //buttonsWidget->lower();
    QList<QString> images;
    images.append(":/image/right.png");
    images.append(":/image/rigut_down.png");
    images.append(":/image/down.png");
    images.append(":/image/left_down.png");
    images.append(":/image/left.png");
    images.append(":/image/left_up.png");
    images.append(":/image/up.png");
    images.append(":/image/right_up.png");
    for (int i = 0; i < 8; ++i) {
        QPushButton *button = new QPushButton(buttonsWidget);
        button->setIcon(QIcon(images[i]));
        button->setFixedSize(30, 30);
        button->setStyleSheet("QPushButton { background-color: #6C6C6C; border: none; border-radius: 15; }");

        int angle = i * 45;
        int x = 100 + std::cos(angle * M_PI / 180.0) * 70;
        int y = 100 + std::sin(angle * M_PI / 180.0) * 70;
        button->move(x - 15, y - 15);
        connect(button, &QPushButton::clicked, [this, i] {
            buttonClicked(i);
        });
    }
//    connect(centerButton, &QPushButton::clicked, [=] {
//        emit centerButtonClicked();
//    });
}
