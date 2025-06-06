// registerwindow.cpp
#include "RegisterWindow.h"
#include <QMessageBox>
#include <QFont>
#include <QSpacerItem>
#include <QIntValidator>

RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent),
    m_usernameAvailable(false)
{
    setupUI();
    m_clientHandler = new ClientHandler(this);
    connect(m_clientHandler, &ClientHandler::usernameCheckedRegister1,
            this, &RegisterWindow::onUsernameChecked);
    connect(m_clientHandler, &ClientHandler::usernameCheckedRegister2,
            this, &RegisterWindow::isRegist);
    connect(m_clientHandler, &ClientHandler::errorOccurred,
            this, &RegisterWindow::onErrorOccurred);
}

RegisterWindow::~RegisterWindow()
{
}

void RegisterWindow::setupUI()
{
    setWindowTitle("用户注册");
    setMinimumSize(350, 250);

    // 设置字体
    QFont font("Microsoft YaHei", 10); // 使用系统默认字体，大小10
    setFont(font);

    // 创建控件
    QLabel *titleLabel = new QLabel("创建新账户", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #2c3e50; }");

    QLabel *usernameLabel = new QLabel("用户名:", this);
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("3-16位字母或数字");
    m_usernameEdit->setMaxLength(16);

    QLabel *passwordLabel = new QLabel("密码:", this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("至少3位字符");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMaxLength(20);

    QLabel *confirmPasswordLabel = new QLabel("确认密码:", this);
    m_confirmPasswordEdit = new QLineEdit(this);
    m_confirmPasswordEdit->setPlaceholderText("再次输入密码");
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setMaxLength(20);

    m_registerButton = new QPushButton("注 册", this);
    m_registerButton->setMinimumWidth(80);
    m_registerButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; padding: 8px; border-radius: 4px; }"
                                   "QPushButton:hover { background-color: #2980b9; }");

    m_cancelButton = new QPushButton("取 消", this);
    m_cancelButton->setMinimumWidth(80);
    m_cancelButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; padding: 8px; border-radius: 4px; }"
                                 "QPushButton:hover { background-color: #7f8c8d; }");

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    // 用户名部分
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(m_usernameEdit, 1);
    mainLayout->addLayout(usernameLayout);

    // 密码部分
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit, 1);
    mainLayout->addLayout(passwordLayout);

    // 确认密码部分
    QHBoxLayout *confirmPasswordLayout = new QHBoxLayout();
    confirmPasswordLayout->addWidget(confirmPasswordLabel);
    confirmPasswordLayout->addWidget(m_confirmPasswordEdit, 1);
    mainLayout->addLayout(confirmPasswordLayout);

    // 添加垂直间距
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // 按钮部分
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(m_registerButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(m_usernameEdit, &QLineEdit::editingFinished, this, &RegisterWindow::checkUsernameAvailability);
    connect(m_registerButton, &QPushButton::clicked, this, &RegisterWindow::handleRegistration);
    connect(m_cancelButton, &QPushButton::clicked, this, &RegisterWindow::handleCancel);
}

void RegisterWindow::checkUsernameAvailability()
{
    QString username = m_usernameEdit->text().trimmed();
    if (!username.isEmpty()) {
        m_clientHandler->checkUsernameAvailable2(username);
    }
}

void RegisterWindow::handleRegistration()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "警告", "所有字段都必须填写");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "警告", "两次输入的密码不一致");
        return;
    }

    if (!m_usernameAvailable) {
        QMessageBox::warning(this, "警告", "用户名不可用");
        return;
    }

    //加密然后传给服务器
    CryptString m_ctypr(password + m_salt);
    QString MD5 = m_ctypr.generateMD5();

    m_clientHandler->sendPassword(username,m_salt,MD5);
}

void RegisterWindow::handleCancel()
{
    reject();
}

void RegisterWindow::onUsernameChecked(bool available, const QString &username, const QString &salt)
{
    m_usernameAvailable = available;
    if (available) {
        qDebug() << username << "可用";
        m_salt = salt;
    } else {
        QMessageBox::warning(this, "警告", "用户名" + username + "已存在");
    }
}

void RegisterWindow::isRegist(bool available)
{
    if(available){
        QMessageBox::information(this, "成功", "注册成功！");
        accept();
    }else{
        QMessageBox::warning(this, "警告", "注册失败！");
    }
}

void RegisterWindow::onErrorOccurred(const QString &error)
{
    m_usernameAvailable = false;
}
