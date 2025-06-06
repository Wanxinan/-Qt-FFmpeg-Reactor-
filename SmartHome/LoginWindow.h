// loginwindow.h
#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ClientHandler.h"

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    void styleButton(QPushButton *button, const QString &normalColor, const QString &hoverColor);

signals:
    void Loginaccept(const QString &username);

private slots:
    void handleLogin();
    void handleCancel();
    void checkUsernameAvailability();
    void onUsernameChecked(bool available, const QString &username, const QString &salt);
    void onPasswordChecked(bool available, const QString &username);
    void onErrorOccurred(const QString &error);

private:
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QPushButton *m_cancelButton;
    ClientHandler *m_clientHandler;
    bool hasUser = false;
    QString m_salt;

    void setupUI();
};

#endif // LOGINWINDOW_H
