QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CircularButtonWidget.cpp \
    ClientHandler.cpp \
    CryptString.cpp \
    FFmegKits.cpp \
    HTTPRequest.cpp \
    LoginWindow.cpp \
    MainWidget.cpp \
    MainWindow.cpp \
    RegisterWindow.cpp \
    VideoWidget.cpp \
    main.cpp

HEADERS += \
    CircularButtonWidget.h \
    ClientHandler.h \
    CryptString.h \
    FFmegKits.h \
    HTTPRequest.h \
    LoginWindow.h \
    MainWidget.h \
    MainWindow.h \
    RegisterWindow.h \
    VideoWidget.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lavcodec
LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lavdevice
LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lavfilter
LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lavformat
LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lavutil
LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lpostproc
LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lswresample
LIBS += -L$$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/lib/x64/ -lswscale

INCLUDEPATH += $$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/include
DEPENDPATH += $$PWD/../../../libffmpeg_4.4.r101753_msvc16_x64/include

RESOURCES += \
    source.qrc
