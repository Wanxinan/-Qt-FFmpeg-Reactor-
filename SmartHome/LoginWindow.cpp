// loginwindow.cpp
#include "LoginWindow.h"
#include "MainWidget.h"
#include <VideoWidget.h>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent)
{
    setupUI();
    m_clientHandler = new ClientHandler(this);
    // 连接信号槽
    //编辑框失去焦点，向服务器发送用户名
    connect(m_usernameEdit, &QLineEdit::editingFinished, this, &LoginWindow::checkUsernameAvailability);
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(m_cancelButton, &QPushButton::clicked, this, &LoginWindow::handleCancel);
    connect(m_clientHandler, &ClientHandler::usernameCheckedLogin1,
            this, &LoginWindow::onUsernameChecked);
    connect(m_clientHandler, &ClientHandler::usernameCheckedLogin2,
            this, &LoginWindow::onPasswordChecked);
    connect(m_clientHandler, &ClientHandler::errorOccurred,
            this, &LoginWindow::onErrorOccurred);
}

LoginWindow::~LoginWindow()
{
    hasUser = false;
}

void LoginWindow::setupUI()
{
    setWindowTitle("登录");

    // 设置窗口背景颜色
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(240, 245, 255)); // 浅蓝色背景
    setPalette(palette);

    // 设置窗口固定大小
    setFixedSize(350, 200);

    // 创建控件
    QLabel *usernameLabel = new QLabel("用户名:", this);
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("输入用户名");
    QLabel *passwordLabel = new QLabel("密码:", this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("输入密码");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_loginButton = new QPushButton("登录", this);
    m_cancelButton = new QPushButton("取消", this);

    // 美化按钮
    styleButton(m_loginButton, "#3498db", "#2980b9");
    styleButton(m_cancelButton, "#e74c3c", "#c0392b");
    m_loginButton->setFixedSize(80, 30);
    m_cancelButton->setFixedSize(80, 30);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // 用户名行布局
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(m_usernameEdit, 1); // 输入框占据剩余空间
    m_usernameEdit->setMinimumHeight(30); // 设置输入框高度

    // 密码行布局
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit, 1); // 输入框占据剩余空间
    m_passwordEdit->setMinimumHeight(30); // 设置输入框高度

    // 按钮行布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addStretch(1);

    mainLayout->addLayout(usernameLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addLayout(buttonLayout);

    // 设置标签样式
    usernameLabel->setStyleSheet("QLabel { font-size: 14px; color: #2c3e50; }");
    passwordLabel->setStyleSheet("QLabel { font-size: 14px; color: #2c3e50; }");
}

// 辅助函数：美化按钮样式
void LoginWindow::styleButton(QPushButton *button, const QString &normalColor, const QString &hoverColor)
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
        "}"
        "QPushButton:hover {"
        "   background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: %2;"
        "   padding-top: 2px;"
        "   padding-bottom: 0px;"
        "}"
    ).arg(normalColor, hoverColor);

    button->setStyleSheet(styleSheet);
}

//登录
void LoginWindow::handleLogin()
{
    if(!hasUser) {
        QMessageBox::warning(this, "警告", "用户名不存在");
        return;
    }
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名和密码不能为空");
        return;
    }
    m_clientHandler->checkPasswordAvailable(username, password, m_salt);
    //QMessageBox::information(this, "成功", "登录成功！");
    //accept();
}

void LoginWindow::handleCancel()
{
    reject();
}

//向服务器发送用户名
void LoginWindow::checkUsernameAvailability()
{
    QString username = m_usernameEdit->text().trimmed();
    if (!username.isEmpty()) {
        // 登录时也可以检查用户名是否存在
        m_clientHandler->checkUsernameAvailable(username);
    }
}

void LoginWindow::onUsernameChecked(bool available, const QString &username, const QString &salt)
{
    if(available) {
        qDebug() << username << "无误";
        hasUser = true;
        m_salt = salt;
    } else {
        QMessageBox::warning(this, "错误", "用户名 " + username + "不存在");
    }
}

void LoginWindow::onPasswordChecked(bool available, const QString &username)
{
    if(available) {
        QMessageBox::information(this, "成功", username + "登录成功！");
        emit Loginaccept(username);
        //delete m_clientHandler;
        accept();
        MainWidget *mw = new MainWidget(username);
        mw->show();
//        VideoWidget *vw = new VideoWidget();
//        vw->show();
    } else {
        QMessageBox::warning(this, "错误", "用户名或密码错误");
    }
}

void LoginWindow::onErrorOccurred(const QString &error)
{
    QMessageBox::warning(this, "错误", "检查用户名时出错: " + error);
}
