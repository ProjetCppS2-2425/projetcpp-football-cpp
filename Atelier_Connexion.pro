QT       += core gui sql
QT +=charts
QT += network
QT += serialport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    arduino.cpp \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    match.cpp \

HEADERS += \
    arduino.h \
    mainwindow.h \
    connection.h \
    match.h \


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QT += charts
