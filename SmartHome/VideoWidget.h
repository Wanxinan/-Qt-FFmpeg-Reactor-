#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QList>

class QTextBrowser;
class QListWidget;
class QPushButton;
class QGroupBox;
class QSlider;
class QSpinBox;

// 视频显示组件（简化版，实际应用中应集成FFmpeg）
class VideoWidget : public QLabel {
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);
    void setVideoSource(const QString &source);
};

class VideoMonitorWidget : public QWidget {
    Q_OBJECT
public:
    explicit VideoMonitorWidget(QWidget *parent = nullptr);

private slots:
    void updateWindowInfo(QTextBrowser *browser);
    void updateAlertList(QListWidget *listWidget);
    void startVideoPoll();
    void ptzControl(const QString &command, int value = -1);

private:
    QList<VideoWidget*> videoLabels;
};

#endif // VIDEOWIDGET_H
