QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        QtService/qtservice.cpp \
        QtService/qtservice_win.cpp \
        main.cpp \
        service.cpp \
        tsysteminfo.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    QtService/QtService \
    QtService/QtServiceBase \
    QtService/QtServiceController \
    QtService/qtservice.h \
    QtService/qtservice_p.h \
    service.h \
    tsysteminfo.h

DISTFILES += \
    ../build-SystemMonitor-Desktop_Qt_6_1_2_MinGW_64_bit-Debug/debug/SystemInfoConfig.ini
