#ifndef CIRCULARBUTTONWIDGET_H
#define CIRCULARBUTTONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <cmath>

class CircularButtonWidget : public QWidget {
    Q_OBJECT

public:
    explicit CircularButtonWidget(QWidget *parent = nullptr);
    ~CircularButtonWidget();

signals:
    void Up();
    void UpRight();
    void Right();
    void DownRight();
    void Down();
    void DownLeft();
    void Left();
    void UpLeft();
    void yuntai(int idx);
    //void centerButtonClicked();

private:
    void setupUI();
    //void createCentralButton(QVBoxLayout *mainLayout);
    void createCircularButtons();
    void buttonClicked(int idx);


    QPushButton *centerButton;
    //QLabel *label;
    QWidget *buttonsWidget;
    QVector<QPushButton*> circularButtons;
};

#endif // CIRCULARBUTTONWIDGET_H
