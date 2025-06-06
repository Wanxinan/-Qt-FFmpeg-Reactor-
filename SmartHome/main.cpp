#include "MainWidget.h"
#include "MainWindow.h"
#include "CircularButtonWidget.h"

#include <QApplication>
#include <QDebug>
extern "C" {
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libpostproc/postprocess.h>
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

//int main(int argc, char *argv[]) {
//    QApplication app(argc, argv);

//    QMainWindow mainWindow;
//    CircularButtonWidget *widget = new CircularButtonWidget();
//    mainWindow.setCentralWidget(widget);
//    mainWindow.resize(300, 300);
//    mainWindow.show();

//    return app.exec();
//}
