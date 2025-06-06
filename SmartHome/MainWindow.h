// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "LoginWindow.h"
#include "RegisterWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void styleButton(QPushButton *button, const QString &normalColor, const QString &hoverColor);
    void openLoginWindow();
private slots:

    void openRegisterWindow();

private:
    QPushButton *m_loginButton;
    QPushButton *m_registerButton;

    LoginWindow *m_loginWindow;
    RegisterWindow *m_registerWindow;


    void setupUI();
};

#endif // MAINWINDOW_H
