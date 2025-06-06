// registerwindow.h
#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H
#include "ClientHandler.h"
#include "CryptString.h"
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void checkUsernameAvailability();
    void handleRegistration();
    void handleCancel();
    void onUsernameChecked(bool available, const QString &username, const QString &salt);
    void isRegist(bool available);
    void onErrorOccurred(const QString &error);

private:
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;
    QPushButton *m_registerButton;
    QPushButton *m_cancelButton;
    ClientHandler *m_clientHandler;
    bool m_usernameAvailable;
    QString m_salt;
    //CryptString m_ctypr;

    void setupUI();
};

#endif // REGISTERWINDOW_H
