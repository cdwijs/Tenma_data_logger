#-------------------------------------------------
#
# Project created by QtCreator 2018-03-12T08:58:41
#
#-------------------------------------------------

QT       += core gui serialport
Release: win32:CONFIG += console #so qDebug talks to windows cmd
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = TenmaDataLogger
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    rs232.cpp \
    settingsdialog.cpp \
    ledindicator.cpp \
    rs232datalogger.cpp \
    filewriter.cpp \

HEADERS += \
    rs232.h \
    settingsdialog.h \
    ledindicator.h \
    rs232datalogger.h \
    filewriter.h \

FORMS += \
    settingsdialog.ui


#git version: https://bitbucket.org/mkarlsso/trodes/src/72c877d43e383ba7daf6e50a634b7cca61f79abd/Modules/SimpleCommunicator/SimpleCommunicator.pro?at=master&fileviewer=file-view-default
GIT_COMMAND = git  describe --always --all --tags --dirty --long --abbrev=40

#must not contain spaces
GIT_COMMIT = $$system($$GIT_COMMAND)
DEFINES += GIT_COMMIT=\\\"$$GIT_COMMIT\\\"

GIT_URL_CMD = git remote get-url origin
GIT_URL = $$system($$GIT_URL_CMD)
DEFINES += GIT_URL=\\\"$$GIT_URL\\\"

#force Qmake to always run: https://stackoverflow.com/questions/12873437/force-qtcreator-to-run-qmake-when-building
Release: qmakeforce.target = dummy
Release: qmakeforce.commands = $$QMAKE_DEL_FILE release\main.o debug\main.o Makefile ##to force rerun of qmake
Release: qmakeforce.depends = FORCE
Release: PRE_TARGETDEPS += $$qmakeforce.target
Release: QMAKE_EXTRA_TARGETS += qmakeforce


#show the value of all defines: https://forum.qt.io/topic/82531/defines-in-qt-creator/9
message($$DEFINES)

RESOURCES += \
    images.qrc

