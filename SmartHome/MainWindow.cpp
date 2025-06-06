// mainwindow.cpp
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_loginWindow(nullptr),
    m_registerWindow(nullptr)
{
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("主页面");
    resize(400, 300);

    // 设置背景颜色
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(240, 245, 255)); // 浅蓝色背景
    setPalette(palette);

    // 创建控件
    m_loginButton = new QPushButton("登录", this);
    m_registerButton = new QPushButton("注册", this);

    // 美化登录按钮
    styleButton(m_loginButton, "#3498db", "#2980b9");
    m_loginButton->setFixedSize(120, 40); // 设置固定大小

    // 美化注册按钮
    styleButton(m_registerButton, "#2ecc71", "#27ae60");
    m_registerButton->setFixedSize(120, 40); // 设置固定大小

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // 添加弹性空间使按钮居中
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(m_registerButton);
    buttonLayout->addStretch(1);

    mainLayout->addLayout(buttonLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 连接信号槽
    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::openLoginWindow);
    connect(m_registerButton, &QPushButton::clicked, this, &MainWindow::openRegisterWindow);
}

// 辅助函数：美化按钮样式
void MainWindow::styleButton(QPushButton *button, const QString &normalColor, const QString &hoverColor)
{
    QString styleSheet = QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   outline: none;"
        "   padding: 5px 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: %2;"
        "   padding-top: 6px;"
        "   padding-bottom: 4px;"
        "}"
    ).arg(normalColor, hoverColor);

    button->setStyleSheet(styleSheet);
}

void MainWindow::openLoginWindow()
{
    if (!m_loginWindow) {
        m_loginWindow = new LoginWindow(this);
        close();
    }

    if (m_loginWindow->exec() == QDialog::Accepted) {
        // 登录成功后的处理
        // 可以打开主应用界面或显示登录成功消息
    }
}

void MainWindow::openRegisterWindow()
{
    if (!m_registerWindow) {
        m_registerWindow = new RegisterWindow(this);
    }

    if (m_registerWindow->exec() == QDialog::Accepted) {
        // 注册成功后的处理
    }
}
